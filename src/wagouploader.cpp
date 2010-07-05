#include "wagouploader.h"

WagoUploader::WagoUploader(QObject *parent):
                QObject(parent), quit(false), timer(NULL)
{
}

WagoUploader::~WagoUploader()
{
        stopUpload();
}

void WagoUploader::startUpload(QString ip)
{
        quit = false;
        commands.clear();
        wago_ip = ip;
        emit statusUpdate(WAGOST_START);

        emit statusUpdate(WAGOST_CREATING);
        createConfig();

        if (quit)
        {
                emit statusUpdate(WAGOST_ABORTED);
                return;
        }

        emit statusUpdate(WAGOST_UPLOADING);
        uploadConfig();
}

void WagoUploader::stopUpload()
{
        quit = true;
}

void WagoUploader::createConfig()
{
        for (int i = 0;i < ListeRule::Instance().size() && !quit;i++)
        {
                Rule *rule = ListeRule::Instance().get_rule(i);

                WagoRuleCommand wcommand;

                //some security checks...
                if (rule->get_size_conds() <= 0) continue;
                if (rule->get_size_actions() <= 0) continue;
                if (rule->get_condition(0)->get_size() <= 0) continue;
                if (rule->get_action(0)->get_size() <= 0) continue;
                //if there is more than one condition we can't generate a simple rule for it,
                //so discard it.
                if (rule->get_condition(0)->get_size() > 1) continue;

                /* Search if the input is valid */
                Input *input = rule->get_condition(0)->get_input(0);

                if (input->get_param("type") != "WIDigitalBP" && input->get_param("type") != "WIDigitalTriple")
                        continue;

                if (input->get_param("type") == "WIDigitalTriple" &&
                    rule->get_condition(0)->get_params().get_param(input->get_param("id")) != "1")
                        continue;

                if (input->get_param("knx") == "true")
                {
                        from_string(input->get_param("var"), wcommand.input);
                        wcommand.input += 256;
                }
                else
                {
                        from_string(input->get_param("var"), wcommand.input);
                }

                /* Get the output */
                Output *output = NULL;
                string val;
                for (int j = 0;j < rule->get_action(0)->get_size();j++)
                {
                        output = rule->get_action(0)->get_output(j);

                        if (output->get_param("type") != "WODigital" && output->get_param("type") != "WOVolet" &&
                            output->get_param("type") != "WOVoletSmart" && output->get_param("type") != "WODali")
                                continue;

                        val = rule->get_action(0)->get_params().get_param(output->get_param("id"));
                        if (val != "toggle" && val != "true") continue;

                        break;
                }
                if (output == NULL) continue;

                if (output->get_param("type") == "WODali")
                {
                        wcommand.addr1 = 1;
                        from_string(output->get_param("address"), wcommand.addr2);

                        if (output->get_param("group") == "1")
                                wcommand.type = TWAGO_DALIGRP;
                        else
                                wcommand.type = TWAGO_DALI;
                }
                else if (output->get_param("type") == "WODigital")
                {
                        from_string(output->get_param("var"), wcommand.addr1);
                        if (output->get_param("knx") == "true")
                        {
                                if (val == "toggle") wcommand.type = TWAGO_TELERUPTEUR_KNX;
                                if (val == "true") wcommand.type = TWAGO_DIRECT_KNX;
                        }
                        else
                        {
                                if (val == "toggle") wcommand.type = TWAGO_TELERUPTEUR;
                                if (val == "true") wcommand.type = TWAGO_DIRECT;
                        }
                }
                else if (output->get_param("type") == "WOVolet" || output->get_param("type") == "WOVoletSmart")
                {
                        from_string(output->get_param("var_up"), wcommand.addr1);
                        from_string(output->get_param("var_down"), wcommand.addr2);

                        int impulse;
                        from_string(output->get_param("impulse_time"), impulse);

                        if (impulse > 0 && output->get_param("impulse_time") != "")
                                wcommand.type = TWAGO_VOLETIMPULSE;
                        else
                                wcommand.type = TWAGO_VOLET;
                }

                //Now search for an already existing rule with that output
                //and set the sameas value if any
                for (int j = 0; j < commands.size(); ++j)
                {
                        const WagoRuleCommand &cmd = commands.at(j);

                        if (cmd.addr1 == wcommand.addr1 &&
                            cmd.addr2 == wcommand.addr2 &&
                            cmd.type == wcommand.type &&
                            cmd.sameas == -1)
                        {
                                //We have one, set sameas and quit loop
                                wcommand.sameas = cmd.input;
                        }
                }

                //push back new rule
                commands << wcommand;

                int percent = (i * 100) / ListeRule::Instance().size();
                emit progressUpdate(percent);

                QCoreApplication::processEvents();
        }
}

void WagoUploader::uploadConfig()
{
        if (timer) delete timer;

        upload_num = 0;

        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(timerUpload()));
        timer->start(200);
}

void WagoUploader::timerUpload()
{
        if (quit || upload_num >= commands.size())
        {
                timer->stop();
                delete timer;
                timer = NULL;

                if (quit)
                        emit statusUpdate(WAGOST_ABORTED);
                else
                        emit statusUpdate(WAGOST_DONE);

                return;
        }

        WagoRuleCommand &cmd = commands[upload_num];
        QString datagram;

        if (!cmd.type_sent)
        {
                datagram = "WAGO_SET_OUTTYPE ";
                datagram += QString::number(cmd.input) + " ";
                datagram += QString::number(cmd.type);
        }
        else
        {
                datagram = "WAGO_SET_OUTADDR ";
                datagram += QString::number(cmd.input) + " ";
                datagram += QString::number(cmd.addr1) + " ";
                datagram += QString::number(cmd.addr2) + " ";
                datagram += QString::number(cmd.sameas);
        }

        WagoConnect::Instance().SendCommand(datagram);

        int percent = (upload_num * 100) / commands.size();
        emit progressUpdate(percent);

        if (cmd.type_sent)
                upload_num++;
        else
                cmd.type_sent = true;
}
