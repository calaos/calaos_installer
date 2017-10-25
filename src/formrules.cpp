#include "formrules.h"
#include "ui_FormRules.h"

#include "TwoLineItemDelegate.h"
#include "mainwindow.h"
#include "DialogEditTimeRange.h"
#include "DialogNewAVReceiver.h"
#include "dialognewwebioshutter.h"
#include "wizards/hue/wizardhue.h"

FormRules::FormRules(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormRules),
    current_room(NULL),
    project_changed(false),
    popupConditionStd(new FormConditionStd(this)),
    popupConditionStart(new FormConditionStart(this)),
    popupConditionScript(new FormConditionScript(this)),
    popupActionStd(new FormActionStd(this)),
    popupActionMail(new FormActionMail(this)),
    popupActionScript(new FormActionScript(this)),
    popupActionTouchscreen(new FormActionTouchscreen(this))
{
    ui->setupUi(this);

    add_menu = new QMenu(parent);
    ui->ioexplorer_add->setMenu(add_menu);

    QAction *action = NULL;

    action = add_menu->addAction(tr("Room"));
    action->setIcon(QIcon(":/img/rooms/various_small.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_ROOM); });

    add_menu->addSeparator();

    QMenu *wago_menu = add_menu->addMenu(QIcon("://img/logo_wago.png"), "Wago PLC");

    action = wago_menu->addAction(tr("Switch"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_INPUT_SWITCH); });

    action = wago_menu->addAction(tr("Light"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_LIGHT); });

    action = wago_menu->addAction(tr("Shutter"));
    action->setIcon(QIcon(":/img/icon_shutter.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_SHUTTER); });

    action = wago_menu->addAction(tr("DALI/DMX"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_DALI); });

    action = wago_menu->addAction(tr("DALI/DMX RGB"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_LIGHT_RGB); });

    action = wago_menu->addAction(tr("Temperature sensor"));
    action->setIcon(QIcon(":/img/temp.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_TEMP); });

    action = wago_menu->addAction(tr("Analog Input/Output"));
    action->setIcon(QIcon(":/img/icon_analog.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WAGO, ITEM_ANALOG); });

    QMenu *owire_menu = add_menu->addMenu(QIcon("://img/chip.png"), "OneWire");

    action = owire_menu->addAction(tr("Temperature sensor"));
    action->setIcon(QIcon(":/img/temp.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "OWTemp" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    QMenu *x10_menu = add_menu->addMenu(QIcon("://img/x10.png"), "X10");

    action = x10_menu->addAction(tr("Light Dimmer"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "X10Output" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    QMenu *zibase_menu = add_menu->addMenu(QIcon("://img/zibase.png"), "Zibase");

    action = zibase_menu->addAction(tr("Temperature sensor"));
    action->setIcon(QIcon(":/img/temp.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "ZibaseTemp" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = zibase_menu->addAction(tr("Switch input"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "ZibaseDigitalIn" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = zibase_menu->addAction(tr("Analog input"));
    action->setIcon(QIcon(":/img/icon_analog.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "ZibaseAnalogIn" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = zibase_menu->addAction(tr("Digital output"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "ZibaseDigitalOut" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    QMenu *mysensors_menu = add_menu->addMenu(QIcon("://img/mysensors.png"), "MySensors");

    action = mysensors_menu->addAction(tr("Switch"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsInputSwitch" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("Switch triple"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsInputSwitchTriple" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("Switch Long press"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsInputSwitchLongPress" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("Light"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsOutputLight" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("Shutter"));
    action->setIcon(QIcon(":/img/icon_shutter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsOutputShutter" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("Smart Shutter"));
    action->setIcon(QIcon(":/img/icon_shutter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsOutputShutterSmart" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("Dimmer"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsOutputDimmer" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("RGB Light"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsOutputLightRGB" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("Temperature sensor"));
    action->setIcon(QIcon(":/img/temp.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsInputTemp" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("Analog Input"));
    action->setIcon(QIcon(":/img/icon_analog.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsInputAnalog" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("Analog Output"));
    action->setIcon(QIcon(":/img/icon_analog.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsOutputAnalog" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("String Input"));
    action->setIcon(QIcon(":/img/text.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsInputString" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = mysensors_menu->addAction(tr("String Output"));
    action->setIcon(QIcon(":/img/text.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MySensorsOutputString" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    QMenu *web_menu = add_menu->addMenu(QIcon("://img/web.png"), "Web");

    action = web_menu->addAction(tr("Switch Input"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_INPUT_SWITCH); });

    action = web_menu->addAction(tr("Light"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_LIGHT); });

    action = web_menu->addAction(tr("RGB Light"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_LIGHT_RGB); });

    action = web_menu->addAction(tr("Shutter"));
    action->setIcon(QIcon(":/img/icon_shutter.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_SHUTTER); });

    action = web_menu->addAction(tr("Temperature sensor"));
    action->setIcon(QIcon(":/img/temp.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_TEMP); });

    action = web_menu->addAction(tr("Analog Input"));
    action->setIcon(QIcon(":/img/icon_analog.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_ANALOG_IN); });

    action = web_menu->addAction(tr("Analog Output"));
    action->setIcon(QIcon(":/img/icon_analog.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_ANALOG_OUT); });

    action = web_menu->addAction(tr("String Input"));
    action->setIcon(QIcon(":/img/text.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_STRINGIN); });

    action = web_menu->addAction(tr("String Output"));
    action->setIcon(QIcon(":/img/text.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_WEB, ITEM_STRINGOUT); });

    QMenu *gpio_menu = add_menu->addMenu(QIcon("://img/chip.png"), "GPIO");

    action = gpio_menu->addAction(tr("Switch input"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "GpioInputSwitch" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = gpio_menu->addAction(tr("Switch triple click"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "GpioInputSwitchTriple" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = gpio_menu->addAction(tr("Switch long press"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "GpioInputSwitchLongPress" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = gpio_menu->addAction(tr("Light"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "GpioOutputSwitch" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = gpio_menu->addAction(tr("Shutter"));
    action->setIcon(QIcon(":/img/icon_shutter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "GpioOutputShutter" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = gpio_menu->addAction(tr("Smart Shutter"));
    action->setIcon(QIcon(":/img/icon_shutter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "GpioOutputShutterSmart" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    QMenu *ola_menu = add_menu->addMenu(QIcon("://img/ola.png"), "Open Lightning Architecture (DMX)");

    action = ola_menu->addAction(tr("Light dimmer"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "OLAOutputLightDimmer" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = ola_menu->addAction(tr("Light RGB"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "OLAOutputLightRGB" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    QMenu *lan_menu = add_menu->addMenu(QIcon("://img/web.png"), "Lan Devices");

    action = lan_menu->addAction(tr("Ping Switch"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "PingInputSwitch" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = lan_menu->addAction(tr("Wake On Lan Output"));
    action->setIcon(QIcon(":/img/icon_tor_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "WOLOutputBool" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    QMenu *knx_menu = add_menu->addMenu(QIcon("://img/knx.png"), "KNX");

    action = knx_menu->addAction(tr("Normal Switch"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXInputSwitch" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = knx_menu->addAction(tr("Switch triple"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXInputSwitchTriple" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = knx_menu->addAction(tr("Switch Long press"));
    action->setIcon(QIcon(":/img/icon_inter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXInputSwitchLongPress" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = knx_menu->addAction(tr("Analog input"));
    action->setIcon(QIcon(":/img/icon_analog.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXInputAnalog" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = knx_menu->addAction(tr("Temperature sensor"));
    action->setIcon(QIcon(":/img/temp.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXInputTemp" },
                    { "io_type", "input" }};
        addCalaosIO(p);
    });

    action = knx_menu->addAction(tr("Analog output"));
    action->setIcon(QIcon(":/img/icon_analog.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXOutputAnalog" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = knx_menu->addAction(tr("Light"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXOutputLight" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = knx_menu->addAction(tr("Light dimmer"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXOutputLightDimmer" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = knx_menu->addAction(tr("Light RGB"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXOutputLightRGB" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = knx_menu->addAction(tr("Shutter"));
    action->setIcon(QIcon(":/img/icon_shutter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXOutputShutter" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    action = knx_menu->addAction(tr("Smart Shutter"));
    action->setIcon(QIcon(":/img/icon_shutter.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "KNXOutputShutterSmart" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    QMenu *milight_menu = add_menu->addMenu(QIcon("://img/milight.png"), "MiLight / LimitlessLed");

    action = milight_menu->addAction(tr("MI-Light RGB bulb"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "MilightOutputLightRGB" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    QMenu *blink_menu = add_menu->addMenu(QIcon("://img/blinkstick.png"), "Blinkstick");

    action = blink_menu->addAction(tr("Blinkstick RGB USB stick"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        Params p = {{ "type", "BlinkstickOutputLightRGB" },
                    { "io_type", "output" }};
        addCalaosIO(p);
    });

    // QMenu *hue_menu = add_menu->addMenu(QIcon("://img/hue.png"), "Philips Hue");



    QMenu *hue_menu = add_menu->addMenu(QIcon("://img/hue.png"), "Hue");

    action = hue_menu->addAction(tr("Hue Wizard"));
    action->setIcon(QIcon(":/img/icon_light_on.png"));
    connect(action, &QAction::triggered, [=]()
    {
        WizardHue dialog(NULL, NULL);
        if (dialog.exec() == QDialog::Accepted)
        {
            for (Params p: dialog.paramsGet())
            {
                p.Add("id", ListeRoom::get_new_id("output_"));
                IOBase *io = ListeRoom::Instance().createOutput(p, current_room);
                addItemOutput(io, current_room, true);
            }
        }
     });

     action = hue_menu->addAction(tr("Philips Hue"));
     action->setIcon(QIcon(":/img/icon_light_on.png"));
     connect(action, &QAction::triggered, [=]()
     {
         Params p = {{ "type", "HueOutputLightRGB" },
                     { "io_type", "output" }};
         addCalaosIO(p);
     });
    
     QMenu *telinfo_menu = add_menu->addMenu(QIcon("://img/teleinfo.png"), "Teleinfo");

     action = telinfo_menu->addAction(tr("TeleinfoInputAnalog"));
     action->setIcon(QIcon(":/img/icon_analog.png"));

     connect(action, &QAction::triggered, [=]()
     {
         Params p = {{ "type", "TeleinfoInputAnalog" },
                     { "io_type", "input" }};
         addCalaosIO(p);
     });

     QMenu *xplsensors_menu = add_menu->addMenu(QIcon("://img/xpl.png"), "xPL");

     action = xplsensors_menu->addAction(tr("Temperature sensor"));
     action->setIcon(QIcon(":/img/temp.png"));
     connect(action, &QAction::triggered, [=]()
     {
         Params p = {{ "type", "xPLInputTemp" },
                     { "io_type", "input" }};
         addCalaosIO(p);
     });

     action = xplsensors_menu->addAction(tr("Switch"));
     action->setIcon(QIcon(":/img/icon_inter.png"));
     connect(action, &QAction::triggered, [=]()
     {
         Params p = {{ "type", "xPLInputSwitch" },
                     { "io_type", "input" }};
         addCalaosIO(p);
     });

     action = xplsensors_menu->addAction(tr("Analog Input"));
     action->setIcon(QIcon(":/img/icon_analog.png"));
     connect(action, &QAction::triggered, [=]()
     {
         Params p = {{ "type", "xPLInputAnalog" },
                     { "io_type", "input" }};
         addCalaosIO(p);
     });

     action = xplsensors_menu->addAction(tr("String Input"));
     action->setIcon(QIcon(":/img/text.png"));
     connect(action, &QAction::triggered, [=]()
     {
         Params p = {{ "type", "xPLInputString" },
                     { "io_type", "input" }};
         addCalaosIO(p);
     });

     action = xplsensors_menu->addAction(tr("Light"));
     action->setIcon(QIcon(":/img/icon_light_on.png"));
     connect(action, &QAction::triggered, [=]()
     {
         Params p = {{ "type", "xPLOutputSwitch" },
                     { "io_type", "output" }};
         addCalaosIO(p);
     });

     action = xplsensors_menu->addAction(tr("Analog Output"));
     action->setIcon(QIcon(":/img/icon_analog.png"));
     connect(action, &QAction::triggered, [=]()
     {
         Params p = {{ "type", "xPLOutputAnalog" },
                     { "io_type", "output" }};
         addCalaosIO(p);
     });

     action = xplsensors_menu->addAction(tr("String Output"));
     action->setIcon(QIcon(":/img/text.png"));
     connect(action, &QAction::triggered, [=]()
     {
         Params p = {{ "type", "xPLOutputString" },
                     { "io_type", "output" }};
         addCalaosIO(p);
     });

    add_menu->addSeparator();

    action = add_menu->addAction(tr("Camera"));
    action->setIcon(QIcon(":/img/icon_camera_on.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_CAMERA); });

    action = add_menu->addAction(tr("Music zone"));
    action->setIcon(QIcon(":/img/icon_sound.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_MUSIC); });

    action = add_menu->addAction(tr("Audio amplifier"));
    action->setIcon(QIcon(":/img/icon_sound.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_AVR); });

    add_menu->addSeparator();

    action = add_menu->addAction(tr("Internal Variable"));
    action->setIcon(QIcon(":/img/text.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_INTERN); });

    action = add_menu->addAction(tr("Scenario variable"));
    action->setIcon(QIcon(":/img/icon_scenario.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_SCENARIO); });

    action = add_menu->addAction(tr("Time/Scheduling Variable"));
    action->setIcon(QIcon(":/img/icon_clock.png"));
    connect(action, &QAction::triggered, [=]() { addCalaosItem(HW_NONE, ITEM_TIME); });

    addConditionMenu = new QMenu(parent);
    ui->bt_condition_add->setMenu(addConditionMenu);

    action = addConditionMenu->addAction(tr("Normal Condition"));
    action->setIcon(QIcon(":/img/icon_rule.png"));
    connect(action, &QAction::triggered, [=]() { addCondition(COND_STD); });

    action = addConditionMenu->addAction(tr("Condition on output event"));
    action->setIcon(QIcon(":/img/icon_rule_out.png"));
    connect(action, &QAction::triggered, [=]() { addCondition(COND_OUTPUT); });

    action = addConditionMenu->addAction(tr("Condition at start"));
    action->setIcon(QIcon(":/img/icon_rule_start.png"));
    connect(action, &QAction::triggered, [=]() { addCondition(COND_START); });

    action = addConditionMenu->addAction(tr("Script Condition"));
    action->setIcon(QIcon(":/img/icon_rule_script.png"));
    connect(action, &QAction::triggered, [=]() { addCondition(COND_SCRIPT); });

    addActionMenu = new QMenu(parent);
    ui->bt_action_add->setMenu(addActionMenu);

    action = addActionMenu->addAction(tr("Normal Action"));
    action->setIcon(QIcon(":/img/icon_rule.png"));
    connect(action, &QAction::triggered, [=]() { addAction(ACTION_STD); });

    action = addActionMenu->addAction(tr("E-Mail Action"));
    action->setIcon(QIcon(":/img/icon_rule_mail.png"));
    connect(action, &QAction::triggered, [=]() { addAction(ACTION_MAIL); });

    action = addActionMenu->addAction(tr("Script Action"));
    action->setIcon(QIcon(":/img/icon_rule_script.png"));
    connect(action, &QAction::triggered, [=]() { addAction(ACTION_SCRIPT); });

    action = addActionMenu->addAction(tr("Touchscreen Action"));
    action->setIcon(QIcon(":/img/icon_rule.png"));
    connect(action, &QAction::triggered, [=]() { addAction(ACTION_TOUCHSCREEN); });


    connect(ui->tree_home, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_tree(QPoint)));
    connect(ui->tree_condition, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_condition(QPoint)));
    connect(ui->tree_action, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_action(QPoint)));
    connect(ui->tree_rules, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup_rule(QPoint)));
}

FormRules::~FormRules()
{
    delete ui;
}

void FormRules::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FormRules::PopulateRoomsTree()
{
    ui->tree_home->setUpdatesEnabled(false);
    QCoreApplication::processEvents();

    QStringList headers;
    headers << tr("Home");
    ui->tree_home->setHeaderLabels(headers);

    ui->tree_condition->header()->setSectionsMovable(false);
    ui->tree_condition->header()->resizeSection(0, 240);
    ui->tree_condition->header()->resizeSection(1, 100);
    ui->tree_condition->header()->resizeSection(2, 240);

    ui->tree_action->header()->setSectionsMovable(false);
    ui->tree_action->header()->resizeSection(0, 240);
    ui->tree_action->header()->resizeSection(1, 240);

    ui->tree_rules->header()->setSectionsMovable(false);
    ui->tree_rules->header()->resizeSection(0, 200);
    ui->tree_rules->header()->resizeSection(1, 200);

    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *room = ListeRoom::Instance().get_room(i);
        bool selected = false;
        if (i == 0) selected = true;
        QTreeWidgetItemRoom *iroom = addItemRoom(room, selected);

        for (int j = 0;j < room->get_size_in();j++)
        {
            IOBase *in = room->get_input(j);

            if (in->get_gui_type() == "switch" ||
                in->get_gui_type() == "switch3" ||
                in->get_gui_type() == "switch_long" ||
                in->get_gui_type() == "time" ||
                in->get_gui_type() == "time_range" ||
                in->get_gui_type() == "temp" ||
                in->get_gui_type() == "analog_in" ||
                in->get_gui_type() == "string_in")
            {
                addItemInput(in, iroom);
            }

            QCoreApplication::processEvents();
        }

        for (int j = 0;j < room->get_size_out();j++)
        {
            IOBase *out = room->get_output(j);
            addItemOutput(out, iroom);

            QCoreApplication::processEvents();
        }
    }

    ui->tree_home->expandAll();

    QCoreApplication::processEvents();
    ui->tree_home->setUpdatesEnabled(true);
}

void FormRules::PopulateRulesTree()
{
    QStringList headers;
    headers << tr("Type") << tr("Name");
    ui->tree_rules->setHeaderLabels(headers);
    ui->tree_condition->header()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tree_action->header()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tree_rules->header()->setSectionResizeMode(QHeaderView::Interactive);

    for (int i = 0;i < ListeRule::Instance().size();i++)
    {
        Rule *rule = ListeRule::Instance().get_rule(i);
        bool selected = false;
        if (i == 0) selected = true;
        addItemRule(rule, selected);

        QCoreApplication::processEvents();
    }
}

void FormRules::ClearProject()
{
    setProjectModified(false);

    ui->tree_home->clear();
    ui->tree_condition->clear_all();
    ui->tree_action->clear_all();
    ui->tree_rules->clear();
    current_room = NULL;
}

void FormRules::setProjectModified(bool modified)
{
    project_changed = modified;

    emit projectModified(project_changed);
}

IOBase *FormRules::addCalaosItemInputSwitch(int item, int hw_type)
{
    IOBase *input = nullptr;
    bool another;

    if (hw_type == HW_WAGO)
    {
        do
        {
            DialogNewWago dialog(item, current_room);
            if (dialog.exec() == QDialog::Accepted)
            {
                another = dialog.wantAnother();
                input = dialog.getInput();
            }
            else
                another = false;
        } while (another);
    }
    else if (hw_type == HW_WEB)
    {
        DialogNewWebIO dialog(current_room, item);
        if (dialog.exec() == QDialog::Accepted)
            input = dialog.getInput();
    }

    return input;
}

IOBase *FormRules::addCalaosItemLight(int item, int hw_type)
{
    bool another;
    IOBase *output = nullptr;

    if (hw_type == HW_WAGO)
    {
        do
        {
            DialogNewWago dialog(item, current_room);
            if (dialog.exec() == QDialog::Accepted)
            {
                another = dialog.wantAnother();
                output = dialog.getOutput();
            }
            else
                another = false;
        } while (another);
    }
    else if (hw_type == HW_WEB)
    {
        DialogNewWebIO dialog(current_room, item);
        if (dialog.exec() == QDialog::Accepted)
            output = dialog.getInput();
    }

    return output;
}

IOBase *FormRules::addCalaosItemString(int item, int hw_type)
{
    IOBase *io = nullptr;

    if (hw_type == HW_WEB)
    {
        DialogNewWebIO dialog(current_room, item);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (dialog.isInputType())
                io = dialog.getInput();
            else
                io = dialog.getOutput();
        }
    }

    return io;
}

IOBase *FormRules::addCalaosItemShutter(int item, int hw_type)
{
    Q_UNUSED(item);
    IOBase *output = nullptr;

    if (hw_type == HW_WAGO)
    {
        DialogNewVolet dialog(current_room);
        if (dialog.exec() == QDialog::Accepted)
            output = dialog.getOutput();
    }
    else if (hw_type == HW_WEB)
    {
        DialogNewWebIOShutter dialog(current_room);
        if (dialog.exec() == QDialog::Accepted)
            output = dialog.getOutput();
    }

    return output;
}

IOBase *FormRules::addCalaosItemDimmer(int item, int hw_type)
{
    Q_UNUSED(item);
    IOBase *output = nullptr;

    if (hw_type == HW_WAGO)
    {
        DialogNewDali dialog(current_room);
        if (dialog.exec() == QDialog::Accepted)
            output = dialog.getOutput();
    }

    return output;
}

IOBase *FormRules::addCalaosItemRGB(int item, int hw_type)
{
    IOBase *output = nullptr;

    if (hw_type == HW_WAGO)
    {
        DialogNewDaliRGB dialog(current_room);
        if (dialog.exec() == QDialog::Accepted)
            output = dialog.getOutput();
    }
    else if (hw_type == HW_WEB)
    {
        DialogNewWebIO dialog(current_room, item);
        if (dialog.exec() == QDialog::Accepted)
            output = dialog.getOutput();
    }

    return output;
}

IOBase *FormRules::addCalaosItemTemp(int item, int hw_type)
{
    IOBase *io = nullptr;

    if (hw_type == HW_WAGO)
    {
        DialogNewTemp dialog(current_room);
        if (dialog.exec() == QDialog::Accepted)
            io = dialog.getInput();
    }
    else if (hw_type == HW_WEB)
    {
        DialogNewWebIO dialog(current_room, item);
        if (dialog.exec() == QDialog::Accepted)
            io = dialog.getInput();
    }

    return io;
}

IOBase *FormRules::addCalaosItemCamera(int item, int hw_type)
{
    Q_UNUSED(item);
    Q_UNUSED(hw_type);
    IOBase *io = nullptr;

    DialogNewCamera dialog(current_room);
    if (dialog.exec() == QDialog::Accepted)
        io = dialog.getOutput();

    return io;
}

IOBase *FormRules::addCalaosItemAudio(int item, int hw_type)
{
    Q_UNUSED(item);
    Q_UNUSED(hw_type);
    IOBase *io = nullptr;

    DialogNewAudio dialog(current_room);
    if (dialog.exec() == QDialog::Accepted)
        io = dialog.getOutput();

    return io;
}

IOBase *FormRules::addCalaosItemAVR(int item, int hw_type)
{
    Q_UNUSED(item);
    Q_UNUSED(hw_type);
    IOBase *io = nullptr;

    DialogNewAVReceiver dialog(current_room);
    if (dialog.exec() == QDialog::Accepted)
        io = dialog.getOutput();

    return io;
}

IOBase *FormRules::addCalaosItemInternal(int item, int hw_type)
{
    Q_UNUSED(hw_type);
    IOBase *io = nullptr;

    if (item == ITEM_INTERN)
    {
        DialogNewInternal dialog(current_room);
        if (dialog.exec() == QDialog::Accepted)
            io = dialog.getOutput();
    }
    else if (item == ITEM_SCENARIO)
    {
        DialogNewScenario dialog(current_room);
        if (dialog.exec() == QDialog::Accepted)
            io = dialog.getOutput();
    }
    else if (item == ITEM_TIME)
    {
        DialogNewTime dialog(current_room);
        if (dialog.exec() == QDialog::Accepted)
            io = dialog.getInput();
    }

    return io;
}

IOBase *FormRules::addCalaosItemAnalog(int item, int hw_type)
{
    IOBase *io = nullptr;

    if (hw_type == HW_WAGO)
    {
        DialogNewAnalog dialog(current_room);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (dialog.isInputType())
                io = dialog.getInput();
            else
                io = dialog.getOutput();
        }
    }
    else if (hw_type == HW_WEB)
    {
        DialogNewWebIO dialog(current_room, item);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (dialog.isInputType())
                io = dialog.getInput();
            else
                io = dialog.getOutput();
        }
    }

    return io;
}

void FormRules::addCalaosItem(int hw_type, int item)
{
    //some tests.
    if (current_room == NULL && item != ITEM_ROOM)
    {
        if (ListeRoom::Instance().size() <= 0)
        {
            QMessageBox::warning(this, tr("Calaos Installer"), tr("You need to add one room at least!"));
            return;
        }
        else
        {
            QMessageBox::warning(this, tr("Calaos Installer"), tr("You must select a room prior adding elements!"));
            return;
        }
    }

    setProjectModified(true);
    IOBase *res = nullptr;

    switch (item)
    {
    case ITEM_ROOM:
    {
        DialogNewRoom dialog;
        if (dialog.exec() == QDialog::Accepted)
        {
            current_room = ListeRoom::Instance().get_room(ListeRoom::Instance().size() - 1);

            //Add the new room to the main list
            Room *room = ListeRoom::Instance().get_room(ListeRoom::Instance().size() - 1);
            addItemRoom(room, true);

            ui->tree_home->expandAll();
        }
        return;
    }
        break;

    case ITEM_INPUT_SWITCH:
        res = addCalaosItemInputSwitch(item, hw_type);
        break;
    case ITEM_LIGHT:
        res = addCalaosItemLight(item, hw_type);
        break;
    case ITEM_SHUTTER:
        res = addCalaosItemShutter(item, hw_type);
        break;
    case ITEM_DALI:
        res = addCalaosItemDimmer(item, hw_type);
        break;
    case ITEM_LIGHT_RGB:
        res = addCalaosItemRGB(item, hw_type);
        break;
    case ITEM_TEMP:
        res = addCalaosItemTemp(item, hw_type);
        break;
    case ITEM_CAMERA:
        res = addCalaosItemCamera(item, hw_type);
        break;
    case ITEM_MUSIC:
        res = addCalaosItemAudio(item, hw_type);
        break;
    case ITEM_AVR:
        res = addCalaosItemAVR(item, hw_type);
        break;
    case ITEM_INTERN:
        res = addCalaosItemInternal(item, hw_type);
        break;
    case ITEM_SCENARIO:
        res = addCalaosItemInternal(item, hw_type);
        break;
    case ITEM_TIME:
        res = addCalaosItemInternal(item, hw_type);
        break;
    case ITEM_ANALOG:
    case ITEM_ANALOG_IN:
    case ITEM_ANALOG_OUT:
        res = addCalaosItemAnalog(item, hw_type);
        break;
    case ITEM_STRINGIN:
    case ITEM_STRINGOUT:
        res = addCalaosItemString(item, hw_type);
        break;
    default:
        QMessageBox::warning(this, tr("Calaos Installer"), QString(tr("Unknown type (%1)")).arg(item));
    }

    if (!res)
        return;

    if (res->is_inout() || res->is_output())
        addItemOutput(res, current_room, true);
    else if (res->is_input())
        addItemInput(res, current_room, true);
}

void FormRules::addCalaosIO(Params &params)
{
    //some tests.
    if (!current_room)
    {
        if (ListeRoom::Instance().size() <= 0)
            QMessageBox::warning(this, tr("Calaos Installer"), tr("You need to add one room at least!"));
        else
            QMessageBox::warning(this, tr("Calaos Installer"), tr("You must select a room prior adding elements!"));
        return;
    }

    params.Add("name", "New IO");
    params.Add("id", ListeRoom::get_new_id("io_"));

    DialogIOProperties d(params);
    if (d.exec() == QDialog::Accepted)
    {
        IOBase *io;
        if (params["io_type"] == "input")
        {
            io = ListeRoom::Instance().createInput(d.getParams(), current_room);
            addItemInput(io, current_room, true);
        }
        else
        {
            io = ListeRoom::Instance().createOutput(d.getParams(), current_room);
            addItemOutput(io, current_room, true);
        }
    }
}

QTreeWidgetItemRoom *FormRules::addItemRoom(Room *room, bool selected)
{
    QTreeWidgetItemRoom *item = new QTreeWidgetItemRoom(room, ui->tree_home);
    item->setSizeHint(0, QSize(24, 24));

    if (selected) ui->tree_home->setCurrentItem(item);

    updateItemInfos(item);

    setProjectModified(true);

    return item;
}

QString FormRules::getIconFromRoom(Room *room)
{
    QString icon;

    if (room->get_type() == "salon" || room->get_type() == "lounge")
        icon = ":/img/rooms/lounge_small.png";
    else if (room->get_type() == "sdb" || room->get_type() == "bathroom")
        icon = ":/img/rooms/bathroom_small.png";
    else if (room->get_type() == "chambre" || room->get_type() == "bedroom")
        icon = ":/img/rooms/bedroom_small.png";
    else if (room->get_type() == "cave" || room->get_type() == "cellar")
        icon = ":/img/rooms/cellar_small.png";
    else if (room->get_type() == "couloir" || room->get_type() == "hall" || room->get_type() == "corridor")
        icon = ":/img/rooms/corridor_small.png";
    else if (room->get_type() == "sam" || room->get_type() == "diningroom")
        icon = ":/img/rooms/diningroom_small.png";
    else if (room->get_type() == "garage")
        icon = ":/img/rooms/garage_small.png";
    else if (room->get_type() == "cuisine" || room->get_type() == "kitchen")
        icon = ":/img/rooms/kitchen_small.png";
    else if (room->get_type() == "bureau" || room->get_type() == "office")
        icon = ":/img/rooms/office_small.png";
    else if (room->get_type() == "exterieur" || room->get_type() == "outside")
        icon = ":/img/rooms/outside_small.png";
    else if (room->get_type() == "misc" || room->get_type() == "divers" || room->get_type() == "various")
        icon = ":/img/rooms/various_small.png";
    else
        icon = ":/img/rooms/various_small.png";

    return icon;
}

void FormRules::updateItemInfos(QTreeWidgetItemRoom *item)
{
    Room *room = item->getRoom();

    item->setData(0, Qt::DisplayRole, QString::fromUtf8(room->get_name().c_str()));

    item->setData(0, Qt::DecorationRole, QIcon(getIconFromRoom(room)));

    QString s = QString::fromUtf8(room->get_name().c_str());
    s += " (" + QString::fromUtf8(room->get_type().c_str()) + ")";
    item->setData(0, Qt::ToolTipRole, s);
    item->setData(0, Qt::StatusTipRole, s);
}

QTreeWidgetItemInput *FormRules::addItemInput(IOBase *in, Room *parent, bool selected)
{
    QTreeWidgetItemIterator it(ui->tree_home);
    while (*it)
    {
        QTreeWidgetItemRoom *item = dynamic_cast<QTreeWidgetItemRoom *>(*it);
        if (item && item->getRoom() == parent)
            return addItemInput(in, item, selected);
        ++it;
    }

    setProjectModified(true);

    return NULL;
}

QTreeWidgetItemInput *FormRules::addItemInput(IOBase *in, QTreeWidgetItemRoom *parent, bool selected)
{
    QTreeWidgetItemInput *item = new QTreeWidgetItemInput(in, parent);

    if (selected) ui->tree_home->setCurrentItem(item);

    updateItemInfos(item);

    setProjectModified(true);

    return item;
}

void FormRules::updateItemInfos(QTreeWidgetItemInput *item)
{
    IOBase *in = item->getInput();

    item->setData(0, Qt::DisplayRole, QString::fromUtf8(in->get_param("name").c_str()));

    string type = in->get_gui_type();
    qDebug() << "Type: " << type.c_str();
    if (type == "switch" ||
        type == "switch3" ||
        type == "switch_long")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_inter.png"));
    else if (type == "temp")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/temp.png"));
    else if (type == "time_range" || type == "time")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_clock.png"));
    else if (type == "analog_in")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_analog.png"));
    else if (type == "string_in")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/text.png"));
    else
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_unknown.png"));

    QString s = QString::fromUtf8(in->get_param("name").c_str());
    s += " (" + QString::fromUtf8(in->get_param("type").c_str()) + ")";
    if (type == "WIDigitalBP" || type == "WIDigitalTriple" || type == "WIDigitalLong" ||
        type == "WITemp" || type == "WIAnalog")
        s += " #" + QString::fromUtf8(in->get_param("var").c_str());

    item->setData(0, Qt::ToolTipRole, s);
    item->setData(0, Qt::StatusTipRole, s);
}

QTreeWidgetItemOutput *FormRules::addItemOutput(IOBase *out, Room *parent, bool selected)
{
    QTreeWidgetItemIterator it(ui->tree_home);
    while (*it)
    {
        QTreeWidgetItemRoom *item = dynamic_cast<QTreeWidgetItemRoom *>(*it);
        if (item && item->getRoom() == parent)
            return addItemOutput(out, item, selected);
        ++it;
    }

    setProjectModified(true);

    return NULL;
}

QTreeWidgetItemOutput *FormRules::addItemOutput(IOBase *out, QTreeWidgetItemRoom *parent, bool selected)
{
    QTreeWidgetItemOutput *item = new QTreeWidgetItemOutput(out, parent);

    if (selected) ui->tree_home->setCurrentItem(item);

    updateItemInfos(item);

    setProjectModified(true);

    return item;
}

void FormRules::updateItemInfos(QTreeWidgetItemOutput *item)
{
    IOBase *out = item->getOutput();

    item->setData(0, Qt::DisplayRole, QString::fromUtf8(out->get_param("name").c_str()));

    string type = out->get_gui_type();
    if (type == "light" || type == "light_dimmer" || type == "light_rgb")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_light_on.png"));
    else if (type == "shutter" || type == "shutter_smart")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_shutter.png"));
    else if (type == "scenario")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_scenario.png"));
    else if (type == "timer")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_clock.png"));
    else if (type == "var_bool")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_bool_on.png"));
    else if (type == "var_int")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_int.png"));
    else if (type == "var_string" || type == "string_out" || type == "string_in")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/text.png"));
    else if (type == "audio" || type == "avreceiver")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_sound.png"));
    else if (type == "camera")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_camera_on.png"));
    else if (type == "analog_out")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_analog.png"));
    else
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_unknown.png"));

    QString s = QString::fromUtf8(out->get_param("name").c_str());
    s += " (" + QString::fromUtf8(out->get_param("type").c_str()) + ")";
    if (type == "WODigital" || type == "WOAnalog")
        s += " #" + QString::fromUtf8(out->get_param("var").c_str());
    if (type == "WOVolet" || type == "WOVoletSmart")
        s += " #" + QString::fromUtf8(out->get_param("var_up").c_str()) +
             " #" + QString::fromUtf8(out->get_param("var_down").c_str());

    item->setData(0, Qt::ToolTipRole, s);
    item->setData(0, Qt::StatusTipRole, s);
}

QTreeWidgetItemRule *FormRules::addItemRule(Rule *rule, bool selected)
{
    QTreeWidgetItemRule *item = new QTreeWidgetItemRule(rule, ui->tree_rules);

    if (selected) ui->tree_rules->setCurrentItem(item);

    updateItemInfos(item);

    setProjectModified(true);

    return item;
}

void FormRules::updateItemInfos(QTreeWidgetItemRule *item)
{
    Rule *rule = item->getRule();

    item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule.png"));
    item->setData(0, Qt::DisplayRole, QString::fromUtf8(rule->get_type().c_str()));
    item->setData(1, Qt::DisplayRole, QString::fromUtf8(rule->get_name().c_str()));
}

QTreeWidgetItem *FormRules::addItemCondition(Condition *condition, bool selected, bool show_popup)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree_condition);

    updateItemCondition(item, condition);

    if (selected)
        ui->tree_condition->setCurrentItem(item);

    if (show_popup)
        on_tree_condition_itemClicked(item, 0);

    setProjectModified(true);

    return item;
}

void FormRules::updateItemCondition(QTreeWidgetItem *item, Condition *condition)
{
    if (condition->getType() == COND_STD)
    {
        string name, oper, value;
        bool isParamValue = false;

        IOBase *input = NULL;
        if (condition->get_size() > 0)
            input = condition->get_input(0);
        if (!input) return;

        string id = input->get_param("id");
        if (input->get_gui_type() == "audio" ||
            input->get_gui_type() == "camera")
            id = input->get_param("iid");

        name = input->get_param("name");
        oper = condition->get_operator().get_param(id);

        if (oper == "INF") oper = "<";
        if (oper == "INF=") oper = "<=";
        if (oper == "SUP") oper = ">";
        if (oper == "SUP=") oper = ">=";

        if (condition->get_params_var().get_param(id) != "")
        {
            string var_id = condition->get_params_var().get_param(id);
            IOBase *in = ListeRoom::Instance().get_input(var_id);
            if (in)
            {
                value = "";

                int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(in));
                string room_name;
                if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

                item->setData(2, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(in->get_param("name").c_str()));
                item->setData(2, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

                isParamValue = true;
            }
            else
            {
                /* Wrong param_var, remove it */
                value = condition->get_params().get_param(id);
                condition->get_params_var().Delete(id);
            }
        }
        else
            value = condition->get_params().get_param(id);

        item->setData(1, Qt::DisplayRole, QString::fromUtf8(oper.c_str()));

        if (!isParamValue)
        {
            item->setData(2, Qt::DisplayRole, QString::fromUtf8(value.c_str()));

            item->setData(2, TwoLineItemDelegate::headerTextRole, QVariant());
            item->setData(2, TwoLineItemDelegate::subHeaderTextRole, QVariant());
        }

        int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(input));
        string room_name;
        if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

        item->setData(0, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(name.c_str()));
        item->setData(0, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

        QString s = "Input: " + QString::fromUtf8(input->get_param("type").c_str()) + " - " + QString::fromUtf8(room_name.c_str());
        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
        item->setData(1, Qt::ToolTipRole, s);
        item->setData(1, Qt::StatusTipRole, s);
        item->setData(2, Qt::ToolTipRole, s);
        item->setData(2, Qt::StatusTipRole, s);

        item->setSizeHint(0, QSize(0, 25));

        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule.png"));
    }
    else if (condition->getType() == COND_OUTPUT)
    {
        string name, oper, value;
        bool isParamValue = false;

        IOBase *output = condition->getOutput();
        if (!output) return;

        string id = output->get_param("id");
        if (output->get_gui_type() == "audio" ||
            output->get_gui_type() == "camera")
            id = output->get_param("iid");

        name = output->get_param("name");
        oper = condition->getOutputOper();

        if (oper == "INF") oper = "<";
        if (oper == "INF=") oper = "<=";
        if (oper == "SUP") oper = ">";
        if (oper == "SUP=") oper = ">=";

        if (condition->getOutputParamVar() != "")
        {
            string var_id = condition->getOutputParamVar();
            IOBase *out = ListeRoom::Instance().get_output(var_id);
            if (out)
            {
                value = "";

                int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(out));
                string room_name;
                if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

                item->setData(2, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(out->get_param("name").c_str()));
                item->setData(2, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

                isParamValue = true;
            }
            else
            {
                /* Wrong param_var, remove it */
                value = condition->getOutputParam();
                condition->setOutputParamVar("");
            }
        }
        else
            value = condition->getOutputParam();

        item->setData(1, Qt::DisplayRole, QString::fromUtf8(oper.c_str()));

        if (!isParamValue)
        {
            item->setData(2, Qt::DisplayRole, QString::fromUtf8(value.c_str()));

            item->setData(2, TwoLineItemDelegate::headerTextRole, QVariant());
            item->setData(2, TwoLineItemDelegate::subHeaderTextRole, QVariant());
        }

        int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(output));
        string room_name;
        if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

        item->setData(0, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(name.c_str()));
        item->setData(0, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

        QString s = "Input: " + QString::fromUtf8(output->get_param("type").c_str()) + " - " + QString::fromUtf8(room_name.c_str());
        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
        item->setData(1, Qt::ToolTipRole, s);
        item->setData(1, Qt::StatusTipRole, s);
        item->setData(2, Qt::ToolTipRole, s);
        item->setData(2, Qt::StatusTipRole, s);

        item->setSizeHint(0, QSize(0, 25));

        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule_out.png"));
    }
    else if (condition->getType() == COND_START)
    {
        item->setData(0, Qt::DisplayRole, tr("Start"));
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule_start.png"));
    }
    else if (condition->getType() == COND_SCRIPT)
    {
        item->setData(0, Qt::DisplayRole, tr("Script"));
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule_script.png"));
    }

    item->treeWidget()->resizeColumnToContents(0);
    item->treeWidget()->resizeColumnToContents(1);
    item->treeWidget()->resizeColumnToContents(2);
}

QTreeWidgetItem *FormRules::addItemAction(Action *action, bool selected, bool show_popup)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree_action);

    updateItemAction(item, action);

    if (selected) ui->tree_action->setCurrentItem(item);

    if (show_popup)
        on_tree_action_itemClicked(item, 0);

    setProjectModified(true);

    return item;
}

void FormRules::updateItemAction(QTreeWidgetItem *item, Action *action)
{
    if (action->getType() == ACTION_STD)
    {
        string name, value;
        bool isParamValue = false;

        IOBase *output = NULL;
        if (action->get_size() > 0)
            output = action->get_output(0);

        if (!output) return;

        string id = output->get_param("id");
        if (output->get_gui_type() == "audio" ||
            output->get_gui_type() == "camera")
            id = output->get_param("oid");

        name = output->get_param("name");

        if (action->get_params_var().get_param(id) != "")
        {
            string var_id = action->get_params_var().get_param(id);
            IOBase *out = ListeRoom::Instance().get_output(var_id);
            if (out)
            {
                value = "";

                int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(out));
                string room_name;
                if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

                item->setData(1, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(out->get_param("name").c_str()));
                item->setData(1, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

                isParamValue = true;
            }
            else
            {
                /* wrong param_var, remove it */
                value = action->get_params().get_param(id);
                action->get_params_var().Delete(id);
            }
        }
        else
            value = action->get_params().get_param(id);

        if (!isParamValue)
        {
            item->setData(1, Qt::DisplayRole, QString::fromUtf8(value.c_str()));

            item->setData(1, TwoLineItemDelegate::headerTextRole, QVariant());
            item->setData(1, TwoLineItemDelegate::subHeaderTextRole, QVariant());
        }

        int r = ListeRoom::Instance().searchIO(dynamic_cast<IOBase *>(output));
        string room_name;
        if (r >= 0) room_name = ListeRoom::Instance().get_room(r)->get_name();

        item->setData(0, TwoLineItemDelegate::headerTextRole, QString::fromUtf8(name.c_str()));
        item->setData(0, TwoLineItemDelegate::subHeaderTextRole, QString::fromUtf8(room_name.c_str()));

        QString s = "Output: " + QString::fromUtf8(output->get_param("type").c_str()) + " - " + QString::fromUtf8(room_name.c_str());
        item->setData(0, Qt::ToolTipRole, s);
        item->setData(0, Qt::StatusTipRole, s);
        item->setData(1, Qt::ToolTipRole, s);
        item->setData(1, Qt::StatusTipRole, s);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setSizeHint(0, QSize(0, 25));

        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule.png"));
    }
    else if (action->getType() == ACTION_MAIL)
    {
        item->setData(0, Qt::DisplayRole, tr("EMail"));
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule_mail.png"));
    }
    else if (action->getType() == ACTION_SCRIPT)
    {
        item->setData(0, Qt::DisplayRole, tr("Script"));
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule_script.png"));
    }
    else if (action->getType() == ACTION_TOUCHSCREEN)
    {
        item->setData(0, Qt::DisplayRole, tr("Touchscreen"));

        QString s, camname = "UNKNOWN Camera!";
        if (action->getTouchscreenAction() == "view_camera")
        {
            IOBase *io = ListeRoom::Instance().get_output(action->getTouchscreenCamera());
            if (io)
                camname = QString::fromUtf8(io->get_param("name").c_str());
            s = QString("View camera '%1'").arg(camname);
        }
        else
        {
            s = "Unknown Touchscreen action!";
        }

        item->setData(1, Qt::DisplayRole, s);
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_rule.png"));
    }

    item->treeWidget()->resizeColumnToContents(0);
    item->treeWidget()->resizeColumnToContents(1);
}

void FormRules::goSelectRule()
{
    if (!treeItem) return;

    QString filter_text = "inout:";

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
    if (itinput)
    {
        string id = itinput->getInput()->get_param("id");
        if (itinput->getInput()->get_gui_type() == "audio" ||
            itinput->getInput()->get_gui_type() == "camera")
            id = itinput->getInput()->get_param("iid");

        filter_text += id.c_str();
    }

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        string id = itoutput->getOutput()->get_param("id");
        if (itoutput->getOutput()->get_gui_type() == "audio" ||
            itoutput->getOutput()->get_gui_type() == "camera")
            id = itoutput->getOutput()->get_param("oid");

        filter_text += id.c_str();
    }

    ui->filterEditRules->setText(filter_text);
}

void FormRules::on_tree_home_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    QTreeWidgetItemRoom *itroom = dynamic_cast<QTreeWidgetItemRoom *>(current);
    if (itroom)
    {
        current_room = itroom->getRoom();
    }

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(current);
    if (itinput)
    {
        itroom = dynamic_cast<QTreeWidgetItemRoom *>(itinput->parent());
        current_room = itroom->getRoom();
    }

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(current);
    if (itoutput)
    {
        itroom = dynamic_cast<QTreeWidgetItemRoom *>(itoutput->parent());
        current_room = itroom->getRoom();
    }
}

void FormRules::showPopup_tree(const QPoint point)
{
    QTreeWidgetItem *item = NULL;
    item = ui->tree_home->itemAt(point);

    if (!item)
    {
        add_menu->exec(QCursor::pos());
    }
    else
    {
        QMenu item_menu(ui->tree_home);

        QAction *action = NULL;

        treeItem = item;

        action = item_menu.addAction(tr("Display the associated rules"));
        action->setIcon(QIcon(":/img/go-last.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(goSelectRule()));

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);

        if (ListeRoom::Instance().size() > 1 &&
            (itoutput || itinput))
        {
            QMenu *moveMenu = item_menu.addMenu(tr("Move to..."));
            Room *current = nullptr;

            if (itinput) current = ListeRoom::Instance().searchRoomByInput(itinput->getInput());
            if (itoutput) current = ListeRoom::Instance().searchRoomByOutput(itoutput->getOutput());

            for (int i = 0;i < ListeRoom::Instance().size();i++)
            {
                Room *room = ListeRoom::Instance().get_room(i);
                if (room == current) continue;

                action = moveMenu->addAction(QString::fromUtf8(room->get_name().c_str()));
                action->setIcon(QIcon(getIconFromRoom(room)));
                action->setData(QVariant::fromValue<void *>(room));

                connect(action, SIGNAL(triggered()), this, SLOT(moveIOToRoom()));
            }
        }

        action = item_menu.addAction(tr("Delete"));
        action->setIcon(QIcon(":/img/user-trash.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(deleteItem()));

        item_menu.addSeparator();

        //Here we have to add Item action (ON/OFF/UP/DOWN/...)
        if (itoutput)
        {
            IOBase *o = itoutput->getOutput();

            if (o->get_param("type") == "WODigital" || o->get_param("type") == "WODali" || o->get_param("type") == "WODaliRVB")
            {
                action = item_menu.addAction(tr("Switch on"));
                action->setIcon(QIcon(":/img/icon_light_on.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemLightOn()));

                action = item_menu.addAction(tr("Switch off"));
                action->setIcon(QIcon(":/img/icon_light_on.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemLightOff()));

                item_menu.addSeparator();
            }

            if (o->get_gui_type() == "camera")
            {
                action = item_menu.addAction(tr("Display the camera view"));
                action->setIcon(QIcon(":/img/icon_camera_on.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemShowCamera()));

                item_menu.addSeparator();
            }


            if (o->get_gui_type() == "shutter" || o->get_gui_type() == "shutter_smart")
            {
                if (o->get_gui_type() == "shutter_smart")
                {
                    action = item_menu.addAction(tr("Convert to normal shutter"));
                    action->setIcon(QIcon(":/img/icon_shutter.png"));
                    connect(action, SIGNAL(triggered()), this, SLOT(itemConvertShutterStandard()));
                }
                else
                {
                    action = item_menu.addAction(tr("Convert to smart shutter"));
                    action->setIcon(QIcon(":/img/icon_shutter.png"));
                    connect(action, SIGNAL(triggered()), this, SLOT(itemConvertShutterSmart()));
                }

                item_menu.addSeparator();

                if (o->get_param("type") == "WOVolet" || o->get_param("type") == "WOVoletSmart")
                {
                    action = item_menu.addAction(tr("Up"));
                    action->setIcon(QIcon(":/img/icon_shutter.png"));
                    connect(action, SIGNAL(triggered()), this, SLOT(itemVoletUp()));

                    action = item_menu.addAction(tr("Down"));
                    action->setIcon(QIcon(":/img/icon_shutter.png"));
                    connect(action, SIGNAL(triggered()), this, SLOT(itemVoletDown()));

                    action = item_menu.addAction(tr("Stop"));
                    action->setIcon(QIcon(":/img/icon_shutter.png"));
                    connect(action, SIGNAL(triggered()), this, SLOT(itemVoletStop()));

                    item_menu.addSeparator();
                }
            }
        }

        if (itinput)
        {
            IOBase *o = itinput->getInput();

            if (o->get_gui_type() == "time_range")
            {
                action = item_menu.addAction(tr("Edit the time ranges..."));
                action->setIcon(QIcon(":/img/icon_clock.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemPlagesHoraires()));

                item_menu.addSeparator();
            }

            if (o->get_gui_type() == "switch")
            {
                action = item_menu.addAction(tr("Convert in triple switch..."));
                action->setIcon(QIcon(":/img/icon_inter.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemConvertInterTriple()));

                action = item_menu.addAction(tr("Convert in long press switch..."));
                action->setIcon(QIcon(":/img/icon_inter.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemConvertInterLong()));

                item_menu.addSeparator();
            }

            if (o->get_gui_type() == "switch3")
            {
                action = item_menu.addAction(tr("Convert in normal switch..."));
                action->setIcon(QIcon(":/img/icon_inter.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemConvertInterBP()));

                action = item_menu.addAction(tr("Convert in long press switch..."));
                action->setIcon(QIcon(":/img/icon_inter.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemConvertInterLong()));

                item_menu.addSeparator();
            }

            if (o->get_gui_type() == "switch_long")
            {
                action = item_menu.addAction(tr("Convert in normal switch..."));
                action->setIcon(QIcon(":/img/icon_inter.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemConvertInterBP()));

                action = item_menu.addAction(tr("Convert in triple switch..."));
                action->setIcon(QIcon(":/img/icon_inter.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemConvertInterTriple()));

                item_menu.addSeparator();
            }

            if (o->get_gui_type() == "temp")
            {
                action = item_menu.addAction(tr("Link a setpoint..."));
                action->setIcon(QIcon(":/img/icon_temp.png"));
                connect(action, SIGNAL(triggered()), this, SLOT(itemTempWizard()));

                item_menu.addSeparator();
            }
        }

        action = item_menu.addAction(tr("Properties"));
        action->setIcon(QIcon(":/img/document-properties.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(showPropertiesItem()));

        item_menu.exec(QCursor::pos());
    }
}

void FormRules::moveIOToRoom()
{
    if (!treeItem) return;

    QAction *action = reinterpret_cast<QAction *>(sender());
    if (!action) return;

    Room *to_room = static_cast<Room *>(action->data().value<void *>());
    if (!to_room) return;

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        IOBase *output = itoutput->getOutput();
        if (output)
        {
            int _i = ListeRoom::Instance().searchIO(output);
            if (_i < 0) return;
            Room *old_room = ListeRoom::Instance().get_room(_i);

            if (old_room == to_room) return;

            //delete old item
            old_room->RemoveOutput(output);

            delete treeItem;

            to_room->AddOutput(output);
            addItemOutput(output, to_room, true);

            return;
        }
    }

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
    if (itinput)
    {
        IOBase *input = itinput->getInput();
        if (input)
        {
            int _i = ListeRoom::Instance().searchIO(input);
            if (_i < 0) return;
            Room *old_room = ListeRoom::Instance().get_room(_i);

            if (old_room == to_room) return;

            //delete old item
            old_room->RemoveInput(input);

            delete treeItem;

            to_room->AddInput(input);
            addItemInput(input, to_room, true);

            return;
        }
    }
}

void FormRules::showPopup_condition(const QPoint point)
{
    QTreeWidgetItem *item = NULL;
    item = ui->tree_condition->itemAt(point);

    if (!item)
        return;

    QMenu item_menu(ui->tree_condition);

    QAction *action = NULL;

    treeItem_condition = item;

    action = item_menu.addAction(tr("Delete"));
    action->setIcon(QIcon(":/img/user-trash.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(deleteItemCondition()));

    item_menu.exec(QCursor::pos());
}

void FormRules::showPopup_action(const QPoint point)
{
    QTreeWidgetItem *item = NULL;
    item = ui->tree_action->itemAt(point);

    if (!item)
        return;

    QMenu item_menu(ui->tree_action);

    QAction *action = NULL;

    treeItem_action = item;

    action = item_menu.addAction(tr("Delete"));
    action->setIcon(QIcon(":/img/user-trash.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(deleteItemAction()));

    item_menu.exec(QCursor::pos());
}

void FormRules::deleteItem()
{
    if (!treeItem) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Calaos Installer"),
                                  QString(tr("Are you sure to delete item \"%1\"")).arg(treeItem->text(0)),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
        return;

    setProjectModified(true);

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
    if (itinput)
    {
        ListeRoom::Instance().deleteIOInput(itinput->getInput());
        delete itinput;

        ui->tree_action->clear_all();
        ui->tree_condition->clear_all();
        ui->tree_rules->clear();

        PopulateRulesTree();
        return;
    }

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        ListeRoom::Instance().deleteIOOutput(itoutput->getOutput());
        delete itoutput;

        ui->tree_action->clear_all();
        ui->tree_condition->clear_all();
        ui->tree_rules->clear();

        PopulateRulesTree();
        return;
    }

    QTreeWidgetItemRoom *itroom = dynamic_cast<QTreeWidgetItemRoom *>(treeItem);
    if (itroom)
    {
        ListeRoom::Instance().Remove(itroom->getRoom());
        delete itroom;

        if (ListeRoom::Instance().size() > 0)
            current_room = ListeRoom::Instance().get_room(ListeRoom::Instance().size() - 1);
        else
            current_room = nullptr;

        ui->tree_action->clear_all();
        ui->tree_condition->clear_all();
        ui->tree_rules->clear();

        PopulateRulesTree();
        return;
    }
}

void FormRules::cloneRule()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Calaos Installer"),
                                  tr("Are you sure to clone the selected rule?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
        return;

    Rule *rule = getCurrentRule();
    if (!rule) return;

    Rule *newrule = rule->duplicate();
    ListeRule::Instance().Add(newrule);
    addItemRule(newrule, true);
}

void FormRules::showPropertiesItem()
{
    Params *p = NULL;
    Params proom, prule;
    int type = OBJ_NONE;
    IOBase *io = nullptr;

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
    if (itinput)
    {
        io = itinput->getInput();
        p = &io->get_params();
        type = OBJ_INPUT;
    }

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        io = itoutput->getOutput();
        p = &io->get_params();
        type = OBJ_OUTPUT;
    }

    QTreeWidgetItemRoom *itroom = dynamic_cast<QTreeWidgetItemRoom *>(treeItem);
    if (itroom)
    {
        proom.Add("name", itroom->getRoom()->get_name());
        proom.Add("type", itroom->getRoom()->get_type());
        proom.Add("hits", to_string(itroom->getRoom()->get_hits()));
        p = &proom;
        type = OBJ_ROOM;
    }

    QTreeWidgetItemRule *itrule = dynamic_cast<QTreeWidgetItemRule *>(treeItem);
    if (itrule)
    {
        prule.Add("name", itrule->getRule()->get_name());
        prule.Add("type", itrule->getRule()->get_type());
        p = &prule;
        type = OBJ_RULE;
    }

    if (!p)
        return;

    if (type == OBJ_ROOM ||
        type == OBJ_RULE)
    {
        DialogListProperties dialog(*p, type, this);
        if (dialog.exec() == QDialog::Accepted)
        {
            *p = dialog.getParams();
            if (type == OBJ_ROOM)
            {
                Room *room = itroom->getRoom();
                string n;
                n = (*p)["name"];
                room->set_name(n);
                n = (*p)["type"];
                room->set_type(n);

                int hits;
                from_string((*p)["hits"], hits);
                room->set_hits(hits);

                updateItemInfos(itroom);
            }
            else if (type == OBJ_RULE)
            {
                Rule *rule = itrule->getRule();
                string n;
                n = (*p)["name"];
                rule->set_name(n);
                n = (*p)["type"];
                rule->set_type(n);

                updateItemInfos(itrule);
            }

            setProjectModified(true);
        }
    }
    else
    {
        DialogIOProperties dialog(*p);
        if (dialog.exec() == QDialog::Accepted)
        {
            *p = dialog.getParams();
            if (type == OBJ_INPUT)
            {
                updateItemInfos(itinput);

                //Handle change of wago_841 parameter and change corresponding cache
                if (itinput->getInput()->get_param("type") == "WIDigitalBP" ||
                    itinput->getInput()->get_param("type") == "WIDigitalTriple" ||
                    itinput->getInput()->get_param("type") == "WIDigitalLong")
                {
                    if (itinput->getInput()->get_param("wago_841") != "true")
                        ProjectManager::wagoTypeCache[itinput->getInput()->get_param("host")] = false;
                    else
                        ProjectManager::wagoTypeCache[itinput->getInput()->get_param("host")] = true;
                }
            }
            else if (type == OBJ_OUTPUT)
            {
                updateItemInfos(itoutput);

                //Handle change of wago_841 parameter and change corresponding cache
                if (itoutput->getOutput()->get_param("type") == "WODigital" ||
                    itoutput->getOutput()->get_param("type") == "WOVolet" ||
                    itoutput->getOutput()->get_param("type") == "WOVoletSmart")
                {
                    if (itoutput->getOutput()->get_param("wago_841") != "true")
                        ProjectManager::wagoTypeCache[itoutput->getOutput()->get_param("host")] = false;
                    else
                        ProjectManager::wagoTypeCache[itoutput->getOutput()->get_param("host")] = true;
                }
            }

            setProjectModified(true);
        }
    }

}

void FormRules::on_tree_home_itemDoubleClicked(QTreeWidgetItem* item, int)
{
    treeItem = item;
    showPropertiesItem();
}

void FormRules::on_bt_rules_add_clicked()
{
    string type = "", name = "";

    if (current_room) type = current_room->get_name();

    if (ui->tree_home->selectedItems().size() > 0)
    {
        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(ui->tree_home->selectedItems().first());
        if (itinput)
        {
            IOBase *input = itinput->getInput();
            name = input->get_param("name");
        }
    }

    DialogNewRule dialog(name, type);
    if (dialog.exec() == QDialog::Accepted)
    {
        Rule *rule = dialog.getRule();
        if (rule)
            addItemRule(rule, true);
        else
            QMessageBox::critical(this, tr("Calaos Installer"), tr("Error when creating the object!"));
    }
}

void FormRules::on_tree_rules_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    //clear both lists
    ui->tree_condition->clear_all();
    ui->tree_action->clear_all();

    QTreeWidgetItemRule *itrule = dynamic_cast<QTreeWidgetItemRule *>(current);
    if (itrule)
    {

        //add new elements from selected rule
        Rule *rule = itrule->getRule();

        for (int i = 0;i < rule->get_size_conds();i++)
        {
            addItemCondition(rule->get_condition(i));
        }

        for (int i = 0;i < rule->get_size_actions();i++)
        {
            addItemAction(rule->get_action(i));
        }
    }
}

Rule *FormRules::getCurrentRule()
{
    QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(ui->tree_rules->currentItem());
    if (item)
        return item->getRule();

    return NULL;
}

void FormRules::on_bt_rules_del_clicked()
{
    if (ui->tree_rules->selectedItems().size() <= 0)
        return;

    QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(ui->tree_rules->selectedItems().first());
    if (item)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calaos Installer"),
                                      tr("Are you sure to delete the rule?"),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            Rule *r = item->getRule();
            ListeRule::Instance().Remove(r);

            delete item;

            //clear both lists
            ui->tree_condition->clear_all();
            ui->tree_action->clear_all();

            if (ui->tree_rules->selectedItems().size() > 0)
            {
                QTreeWidgetItem *item = ui->tree_rules->selectedItems().first();
                ui->tree_rules->setCurrentItem(NULL);
                ui->tree_rules->setCurrentItem(item);
            }

            setProjectModified(true);
        }
    }
}

void FormRules::itemLightOn()
{
    if (!treeItem) return;

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        if (itoutput->getOutput()->get_param("type") == "WODigital")
        {
            QString cmd;

            if (itoutput->getOutput()->get_param("knx") == "true")
                cmd = "WAGO_SET_KNX_OUTPUT ";
            else
                cmd = "WAGO_SET_OUTPUT ";
            cmd += itoutput->getOutput()->get_param("var").c_str();
            cmd += " 1";

            WagoConnect::Instance().SendCommand(cmd);
        }
        else if (itoutput->getOutput()->get_param("type") == "WODali")
        {
            QString cmd = "WAGO_DALI_SET 1 ";
            if (itoutput->getOutput()->get_param("group") == "1")
                cmd += "1 ";
            else
                cmd += "0 ";

            cmd += itoutput->getOutput()->get_param("address").c_str();
            cmd += " 100 ";
            cmd += itoutput->getOutput()->get_param("fade_time").c_str();

            WagoConnect::Instance().SendCommand(cmd);
        }
    }
}

void FormRules::itemLightOff()
{
    if (!treeItem) return;

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        if (itoutput->getOutput()->get_param("type") == "WODigital")
        {
            QString cmd;

            if (itoutput->getOutput()->get_param("knx") == "true")
                cmd = "WAGO_SET_KNX_OUTPUT ";
            else
                cmd = "WAGO_SET_OUTPUT ";
            cmd += itoutput->getOutput()->get_param("var").c_str();
            cmd += " 0";

            WagoConnect::Instance().SendCommand(cmd);
        }
        else if (itoutput->getOutput()->get_param("type") == "WODali")
        {
            QString cmd = "WAGO_DALI_SET 1 ";
            if (itoutput->getOutput()->get_param("group") == "1")
                cmd += "1 ";
            else
                cmd += "0 ";

            cmd += itoutput->getOutput()->get_param("address").c_str();
            cmd += " 0 ";
            cmd += itoutput->getOutput()->get_param("fade_time").c_str();

            WagoConnect::Instance().SendCommand(cmd);
        }
    }
}

void FormRules::itemShowCamera()
{
    if (!treeItem) return;

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        if (itoutput->getOutput()->get_gui_type() == "camera")
        {
            DialogCameraView d(itoutput->getOutput());
            d.DownloadPicture();
            d.exec();
        }
    }
}

void FormRules::itemPlagesHoraires()
{
    if (!treeItem) return;

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
    if (itinput)
    {
        if (itinput->getInput()->get_gui_type() == "time_range")
        {
            DialogEditTimeRange d(itinput->getInput());
            if (d.exec() == QDialog::Accepted)
            {
                d.save();
                setProjectModified(true);
            }
        }
    }
}

void FormRules::itemConvertInterTriple()
{
    if (!treeItem) return;

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
    if (itinput)
    {
        if (itinput->getInput()->get_gui_type() == "switch" ||
            itinput->getInput()->get_gui_type() == "switch_long")
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Calaos Installer"),
                                          tr("Are you sure to convert this switch into a triple click switch?"),
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                IOBase *input = itinput->getInput();

                QString old_type = QString::fromUtf8(input->get_gui_type().c_str());

                //Change WAGO
                if (input->get_param("type") == "WIDigitalBP" ||
                    input->get_param("type") == "WIDigitalLong")
                    input->get_params().Add("type", "WIDigitalTriple");

                //Change GPIO
                else if (input->get_param("type") == "GpioInputSwitch" ||
                         input->get_param("type") == "GpioInputSwitchLongPress")
                    input->get_params().Add("type", "GpioInputSwitchTriple");

                else
                {
                    QMessageBox::warning(this, tr("Calaos Installer"), tr("Sorry, this is not implemented !"));
                    return;
                }

                input->set_gui_type("switch3");
                updateItemInfos(itinput);

                goSelectRule();

                if (old_type != "switch")
                {
                    setProjectModified(true);
                    return;
                }

                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                              tr("Do you want to automatically convert the rules?"),
                                              QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                {
                    QTreeWidgetItemIterator it(ui->tree_rules);
                    while (*it)
                    {
                        QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
                        if (!item)
                        {
                            ++it;
                            continue;
                        }

                        Rule *rule = item->getRule();

                        for (int i = 0;i < rule->get_size_conds();i++)
                        {
                            Condition *cond = rule->get_condition(i);
                            for (int j = 0;j < cond->get_size();j++)
                            {
                                if (input == cond->get_input(j))
                                {
                                    cond->get_params().Add(input->get_param("id"), "1");
                                }
                            }
                        }

                        ++it;
                    }

                    //refresh ui
                    on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);
                }

                setProjectModified(true);
            }
        }
    }
}

void FormRules::itemConvertInterBP()
{
    if (!treeItem) return;

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
    if (itinput)
    {
        if (itinput->getInput()->get_gui_type() == "switch3" ||
            itinput->getInput()->get_gui_type() == "switch_long")
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Calaos Installer"),
                                          tr("Are you sure to convert this switch into a standard switch?"),
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                IOBase *input = itinput->getInput();

                //Change WAGO
                if (input->get_param("type") == "WIDigitalTriple" ||
                    input->get_param("type") == "WIDigitalLong")
                    input->get_params().Add("type", "WIDigitalBP");

                //Change GPIO
                else if (input->get_param("type") == "GpioInputSwitchTriple" ||
                         input->get_param("type") == "GpioInputSwitchLongPress")
                    input->get_params().Add("type", "GpioInputSwitch");

                else
                {
                    QMessageBox::warning(this, tr("Calaos Installer"), tr("Sorry, this is not implemented !"));
                    return;
                }


                input->set_gui_type("switch");
                updateItemInfos(itinput);

                goSelectRule();

                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                              tr("Do you want to automatically convert the rules?"),
                                              QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                {
                    bool more_click_found = false;

                    QTreeWidgetItemIterator it(ui->tree_rules);
                    while (*it)
                    {
                        QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
                        if (!item)
                        {
                            ++it;
                            continue;
                        }

                        Rule *rule = item->getRule();

                        for (int i = 0;i < rule->get_size_conds();i++)
                        {
                            Condition *cond = rule->get_condition(i);
                            for (int j = 0;j < cond->get_size();j++)
                            {
                                if (input == cond->get_input(j))
                                {
                                    if (cond->get_params().get_param(input->get_param("id")) == "1")
                                        cond->get_params().Add(input->get_param("id"), "true");
                                    else
                                        more_click_found = true;
                                }
                            }
                        }

                        ++it;
                    }

                    if (more_click_found)
                    {
                        QMessageBox::warning(this, tr("Calaos Installer"), tr("Warning, the rules with double/triple were not converted, do it manually."));
                    }

                    //refresh ui
                    on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);
                }


                setProjectModified(true);
            }
        }
    }
}

void FormRules::itemConvertInterLong()
{
    if (!treeItem) return;

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
    if (itinput)
    {
        if (itinput->getInput()->get_gui_type() == "switch" ||
            itinput->getInput()->get_gui_type() == "switch3")
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Calaos Installer"),
                                          tr("Are you sure to convert this switch into a long press switch?"),
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                IOBase *input = itinput->getInput();

                QString old_type = QString::fromUtf8(input->get_gui_type().c_str());

                //Change WAGO
                if (input->get_param("type") == "WIDigitalBP" ||
                    input->get_param("type") == "WIDigitalTriple")
                    input->get_params().Add("type", "WIDigitalLong");

                //Change GPIO
                else if (input->get_param("type") == "GpioInputSwitch" ||
                         input->get_param("type") == "GpioInputSwitchTriple")
                    input->get_params().Add("type", "GpioInputSwitchLongPress");

                else
                {
                    QMessageBox::warning(this, tr("Calaos Installer"), tr("Sorry, this is not implemented !"));
                    return;
                }

                input->set_gui_type("switch_long");
                updateItemInfos(itinput);

                goSelectRule();

                if (old_type != "switch")
                {
                    setProjectModified(true);
                    return;
                }

                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, tr("Calaos Installer"),
                                              tr("Do you want to automatically convert the rules?"),
                                              QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                {
                    QTreeWidgetItemIterator it(ui->tree_rules);
                    while (*it)
                    {
                        QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
                        if (!item)
                        {
                            ++it;
                            continue;
                        }

                        Rule *rule = item->getRule();

                        for (int i = 0;i < rule->get_size_conds();i++)
                        {
                            Condition *cond = rule->get_condition(i);
                            for (int j = 0;j < cond->get_size();j++)
                            {
                                if (input == cond->get_input(j))
                                {
                                    cond->get_params().Add(input->get_param("id"), "1");
                                }
                            }
                        }

                        ++it;
                    }

                    //refresh ui
                    on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);
                }

                setProjectModified(true);
            }
        }
    }
}

void FormRules::itemTempWizard()
{
    if (!treeItem) return;

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(treeItem);
    if (itinput)
    {
        if (itinput->getInput()->get_gui_type() == "temp")
        {
            TempWizard wizard;

            if (wizard.exec() == QDialog::Accepted)
            {
                setProjectModified(true);

                QString name = wizard.field("consigneName").toString();
                bool create_rules = wizard.field("checkRules").toBool();

                //Search for an InternalInt or create one
                Room *room = ListeRoom::Instance().searchRoomByInput(itinput->getInput());

                if (!room)
                {
                    QMessageBox::critical(this, tr("Calaos Installer"), tr("Internal error!"));

                    return;
                }

                IOBase *consigne = NULL;
                for (int i = 0;i < room->get_size_in();i++)
                {
                    IOBase *in = room->get_input(i);
                    if (in->get_gui_type() == "var_int" &&
                        in->get_param("name") == name.toUtf8().data())
                    {
                        consigne = in;

                        break;
                    }
                }

                if (!consigne)
                {
                    Params p;
                    p.Add("name", name.toUtf8().constData());
                    p.Add("type", "InternalInt");

                    consigne = ListeRoom::Instance().createInput(p, room);

                    if (consigne->is_output()) addItemOutput(consigne, room, true);
                }

                if (!consigne)
                {
                    QMessageBox::critical(this, tr("Calaos Installer"), tr("Internal error!"));

                    return;
                }

                //Link both together
                int num = 0;
                while (ListeRoom::Instance().get_chauffage_var(Utils::to_string(num), CONSIGNE))
                {
                    num++;
                }

                consigne->set_param("chauffage_id", Utils::to_string(num));
                itinput->getInput()->set_param("chauffage_id", Utils::to_string(num));

                consigne->set_param("value", "20");
                consigne->set_param("rw", "true");
                consigne->set_param("visible", "true");

                //Do we need to create the rules?
                if (create_rules)
                {
                    //The rule (if input <= consigne)
                    {
                        Params p;
                        p.Add("name", room->get_name());
                        p.Add("type", "heat on");
                        Rule *rule = new Rule(p);
                        addItemRule(rule, true);

                        Condition *cond = new Condition(COND_STD);
                        cond->Add(itinput->getInput());
                        cond->get_params().Add(itinput->getInput()->get_param("id"), "");
                        cond->get_operator().Add(itinput->getInput()->get_param("id"), "INF=");
                        cond->get_params_var().Add(itinput->getInput()->get_param("id"),
                                                   consigne->get_param("id"));

                        rule->AddCondition(cond);
                        addItemCondition(cond, true, true);

                        ListeRule::Instance().Add(rule);
                    }

                    //The rule (if input > consigne)
                    {
                        Params p;
                        p.Add("name", room->get_name());
                        p.Add("type", "heat off");
                        Rule *rule = new Rule(p);
                        addItemRule(rule, true);

                        Condition *cond = new Condition(COND_STD);
                        cond->Add(itinput->getInput());
                        cond->get_params().Add(itinput->getInput()->get_param("id"), "");
                        cond->get_operator().Add(itinput->getInput()->get_param("id"), "SUP");
                        cond->get_params_var().Add(itinput->getInput()->get_param("id"),
                                                   consigne->get_param("id"));

                        rule->AddCondition(cond);
                        addItemCondition(cond, true, true);

                        ListeRule::Instance().Add(rule);
                    }
                }
            }
        }
    }
}

void FormRules::itemVoletUp()
{
    if (!treeItem) return;

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        if (itoutput->getOutput()->get_param("type") == "WOVolet" ||
            itoutput->getOutput()->get_param("type") == "WOVoletSmart")
        {
            QString cmd = "WAGO_SET_OUTPUT ";
            cmd += itoutput->getOutput()->get_param("var_down").c_str();
            cmd += " 0";

            WagoConnect::Instance().SendCommand(cmd);

            cmd = "WAGO_SET_OUTPUT ";
            cmd += itoutput->getOutput()->get_param("var_up").c_str();
            cmd += " 1";

            WagoConnect::Instance().SendCommand(cmd);
        }
    }
}

void FormRules::itemVoletDown()
{
    if (!treeItem) return;

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        if (itoutput->getOutput()->get_param("type") == "WOVolet" ||
            itoutput->getOutput()->get_param("type") == "WOVoletSmart")
        {
            QString cmd = "WAGO_SET_OUTPUT ";
            cmd += itoutput->getOutput()->get_param("var_up").c_str();
            cmd += " 0";

            WagoConnect::Instance().SendCommand(cmd);

            cmd = "WAGO_SET_OUTPUT ";
            cmd += itoutput->getOutput()->get_param("var_down").c_str();
            cmd += " 1";

            WagoConnect::Instance().SendCommand(cmd);
        }
    }
}

void FormRules::itemVoletStop()
{
    if (!treeItem) return;

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        if (itoutput->getOutput()->get_param("type") == "WOVolet" ||
            itoutput->getOutput()->get_param("type") == "WOVoletSmart")
        {
            QString cmd = "WAGO_SET_OUTPUT ";
            cmd += itoutput->getOutput()->get_param("var_down").c_str();
            cmd += " 0";

            WagoConnect::Instance().SendCommand(cmd);

            cmd = "WAGO_SET_OUTPUT ";
            cmd += itoutput->getOutput()->get_param("var_up").c_str();
            cmd += " 0";

            WagoConnect::Instance().SendCommand(cmd);
        }
    }
}

void FormRules::itemConvertShutterSmart()
{
    if (!treeItem) return;

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        if (itoutput->getOutput()->get_gui_type() == "shutter")
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Calaos Installer"),
                                          tr("Do you want to convert to a smart shutter?"),
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes && itoutput->getOutput())
            {
                IOBase *out = itoutput->getOutput();

                if (out->get_param("type") == "WOVolet")
                {
                    out->get_params().Add("type", "WOVoletSmart");
                    if (!out->get_params().Exists("time_up"))
                        out->get_params().Add("time_up", out->get_param("time"));
                    if (!out->get_params().Exists("time_down"))
                        out->get_params().Add("time_down", out->get_param("time"));
                    if (!out->get_params().Exists("var_save")) out->get_params().Add("var_save", ListeRoom::get_new_varsave());
                }
                else
                {
                    QMessageBox::warning(this, tr("Calaos Installer"), tr("Sorry, this is not implemented !"));
                    return;
                }

                out->set_gui_type("shutter_smart");
                updateItemInfos(itoutput);
                setProjectModified(true);
            }
        }
    }
}

void FormRules::itemConvertShutterStandard()
{
    if (!treeItem) return;

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(treeItem);
    if (itoutput)
    {
        if (itoutput->getOutput()->get_gui_type() == "shutter_smart")
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Calaos Installer"),
                                          tr("Do you want to convert to normal shutter?"),
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes && itoutput->getOutput())
            {
                IOBase *out = itoutput->getOutput();

                if (out->get_param("type") == "WOVoletSmart")
                {
                    out->get_params().Add("type", "WOVolet");
                    if (!out->get_params().Exists("time"))
                        out->get_params().Add("time", out->get_param("time_up"));

                    out->get_params().Delete("var_save");
                }
                else
                {
                    QMessageBox::warning(this, tr("Calaos Installer"), tr("Sorry, this is not implemented !"));
                    return;
                }

                out->set_gui_type("shutter");

                updateItemInfos(itoutput);
                setProjectModified(true);
            }
        }
    }
}

void FormRules::on_filterEditHome_textChanged(QString filter_text)
{
    if (filter_text.isEmpty())
    {
        QTreeWidgetItemIterator it(ui->tree_home);
        while (*it)
        {
            QTreeWidgetItem *item = (*it);
            item->setHidden(false);
            ++it;
        }

        return;
    }

    QTreeWidgetItemIterator it(ui->tree_home);
    while (*it)
    {
        QTreeWidgetItemRoom *item = dynamic_cast<QTreeWidgetItemRoom *>(*it);
        if (!item)
        {
            ++it;
            continue;
        }

        bool hideItem = true;

        QStringList searchList;
        searchList.push_back(item->getRoom()->get_name().c_str());
        searchList.push_back(item->getRoom()->get_type().c_str());

        if (!searchList.filter(filter_text, Qt::CaseInsensitive).isEmpty())
            hideItem = false;

        item->setHidden(hideItem);

        ++it;
    }
}

void FormRules::on_filterEditRules_textChanged(QString filter_text)
{
    if (filter_text.isEmpty())
    {
        QTreeWidgetItemIterator it(ui->tree_rules);
        while (*it)
        {
            QTreeWidgetItem *item = (*it);
            item->setHidden(false);
            ++it;
        }

        return;
    }

    QString filter_type = filter_text.section(":", 0, 0);

    QTreeWidgetItemRule *item_selected = NULL;

    if (filter_type.toLower() == "output" ||
        filter_type.toLower() == "input" ||
        filter_type.toLower() == "inout")
    {
        filter_text.remove(0, filter_type.length() + 1);

        QTreeWidgetItemIterator it(ui->tree_rules);
        while (*it)
        {
            QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
            if (!item)
            {
                ++it;
                continue;
            }

            Rule *rule = item->getRule();

            QStringList searchList;

            if (filter_type.toLower() == "output" ||
                filter_type.toLower() == "inout")
            {
                for (int i = 0;i < rule->get_size_actions();i++)
                {
                    Action *action = rule->get_action(i);
                    for (int j = 0;j < action->get_size();j++)
                    {
                        searchList << action->get_output(j)->get_param("name").c_str();

                        string id = action->get_output(j)->get_param("id");
                        if (action->get_output(j)->get_gui_type() == "audio" ||
                            action->get_output(j)->get_gui_type() == "camera")
                            id = action->get_output(j)->get_param("oid");

                        searchList << id.c_str();
                    }
                }
            }

            if (filter_type.toLower() == "input" ||
                filter_type.toLower() == "inout")
            {
                for (int i = 0;i < rule->get_size_conds();i++)
                {
                    Condition *cond = rule->get_condition(i);
                    for (int j = 0;j < cond->get_size();j++)
                    {
                        searchList << cond->get_input(j)->get_param("name").c_str();

                        string id = cond->get_input(j)->get_param("id");
                        if (cond->get_input(j)->get_gui_type() == "audio" ||
                            cond->get_input(j)->get_gui_type() == "camera")
                            id = cond->get_input(j)->get_param("iid");

                        searchList << id.c_str();
                    }
                }
            }

            bool hideItem = true;

            QString str;
            foreach (str, searchList)
            {
                if (str == filter_text)
                    hideItem = false;
            }

            item->setHidden(hideItem);

            if (!hideItem && !item_selected)
                item_selected = item;


            ++it;
        }

        ui->tree_rules->setCurrentItem(item_selected);

        return;
    }

    QTreeWidgetItemIterator it(ui->tree_rules);
    while (*it)
    {
        QTreeWidgetItemRule *item = dynamic_cast<QTreeWidgetItemRule *>(*it);
        if (!item)
        {
            ++it;
            continue;
        }

        bool hideItem = true;

        QStringList searchList;
        searchList.push_back(item->getRule()->get_name().c_str());
        searchList.push_back(item->getRule()->get_type().c_str());

        if (!searchList.filter(filter_text, Qt::CaseInsensitive).isEmpty())
            hideItem = false;

        item->setHidden(hideItem);

        if (!hideItem && !item_selected)
            item_selected = item;

        ++it;
    }

    ui->tree_rules->setCurrentItem(item_selected);
}

void FormRules::on_tree_condition_itemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    Rule *rule = getCurrentRule();
    if (!rule) return;

    int num = ui->tree_condition->invisibleRootItem()->indexOfChild(ui->tree_condition->currentItem());

    if (num < 0 || num >= rule->get_size_conds())
        return;

    Condition *cond = rule->get_condition(num);

    setProjectModified(true);

    switch (cond->getType())
    {
    case COND_STD:
    case COND_OUTPUT:
    {
        popupConditionStd->setCondition(item, rule, cond);
        popupConditionStd->layout()->update();
        popupConditionStd->move(ui->tree_condition->mapToGlobal(QPoint(0 - popupConditionStd->width(), 0)));
        popupConditionStd->setFocus();
        popupConditionStd->show();

        break;
    }
    case COND_START:
    {
        popupConditionStart->setCondition(item, rule, cond);
        popupConditionStart->layout()->update();
        popupConditionStart->move(ui->tree_condition->mapToGlobal(QPoint(0 - popupConditionStart->width(), 0)));
        popupConditionStart->setFocus();
        popupConditionStart->show();

        break;
    }
    case COND_SCRIPT:
    {
        popupConditionScript->setCondition(item, rule, cond);
        popupConditionScript->layout()->update();
        popupConditionScript->move(ui->tree_condition->mapToGlobal(QPoint(0 - popupConditionScript->width(), 0)));
        popupConditionScript->setFocus();
        popupConditionScript->show();

        break;
    }
    }

}

void FormRules::on_tree_action_itemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    Rule *rule = getCurrentRule();
    if (!rule) return;

    int num = ui->tree_action->invisibleRootItem()->indexOfChild(ui->tree_action->currentItem());

    if (num < 0 || num >= rule->get_size_actions())
        return;

    Action *action = rule->get_action(num);

    setProjectModified(true);

    switch (action->getType())
    {
    case ACTION_STD:
    {
        popupActionStd->setAction(item, rule, action);
        popupActionStd->layout()->update();
        popupActionStd->move(ui->tree_action->mapToGlobal(QPoint(0 - popupActionStd->width(), 0)));
        popupActionStd->setFocus();
        popupActionStd->show();

        break;
    }
    case ACTION_MAIL:
    {
        popupActionMail->setAction(item, rule, action);
        popupActionMail->layout()->update();
        popupActionMail->move(ui->tree_action->mapToGlobal(QPoint(0 - popupActionMail->width(), 0)));
        popupActionMail->setFocus();
        popupActionMail->show();

        break;
    }
    case ACTION_SCRIPT:
    {
        popupActionScript->setAction(item, rule, action);
        popupActionScript->layout()->update();
        popupActionScript->move(ui->tree_action->mapToGlobal(QPoint(0 - popupActionScript->width(), 0)));
        popupActionScript->setFocus();
        popupActionScript->show();

        break;
    }
    case ACTION_TOUCHSCREEN:
    {
        popupActionTouchscreen->setAction(item, rule, action);
        popupActionTouchscreen->layout()->update();
        popupActionTouchscreen->move(ui->tree_action->mapToGlobal(QPoint(0 - popupActionTouchscreen->width(), 0)));
        popupActionTouchscreen->setFocus();
        popupActionTouchscreen->show();

        break;
    }
    }

}

void FormRules::on_bt_condition_del_clicked()
{
    treeItem_condition = ui->tree_condition->currentItem();

    deleteItemCondition();

    setProjectModified(true);
}

void FormRules::on_bt_action_del_clicked()
{
    treeItem_action = ui->tree_action->currentItem();

    deleteItemAction();

    setProjectModified(true);
}

void FormRules::showPopup_rule(const QPoint point)
{
    QTreeWidgetItem *item = NULL;
    item = ui->tree_rules->itemAt(point);

    if (!item)
        return;

    treeItem = item;

    QMenu item_menu(ui->tree_rules);

    QAction *action = NULL;

    action = item_menu.addAction(tr("Clone rule"));
    action->setIcon(QIcon(":/img/clone.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(cloneRule()));
    action = item_menu.addAction(tr("Properties"));
    action->setIcon(QIcon(":/img/document-properties.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(showPropertiesItem()));
    item_menu.addSeparator();
    action = item_menu.addAction(tr("Delete"));
    action->setIcon(QIcon(":/img/user-trash.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(on_bt_rules_del_clicked()));

    item_menu.exec(QCursor::pos());
}

void FormRules::on_tree_rules_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    treeItem = ui->tree_rules->currentItem();
    showPropertiesItem();
}

void FormRules::addCondition(int type)
{
    if (type == COND_STD)
    {
        IOBase *input = NULL;
        QTreeWidgetItemInput *initem = dynamic_cast<QTreeWidgetItemInput *>(ui->tree_home->currentItem());
        if (!initem)
        {
            QTreeWidgetItemOutput *outitem = dynamic_cast<QTreeWidgetItemOutput *>(ui->tree_home->currentItem());
            if (outitem)
            {
                string type = outitem->getOutput()->get_gui_type();
                if (type == "timer" || type == "scenario" ||
                    type == "var_bool" || type == "var_int" ||
                    type == "var_string")
                {
                    input = ListeRoom::Instance().get_input(outitem->getOutput()->get_param("id"));
                }

                if (outitem->getOutput()->get_gui_type() == "audio")
                    input = ListeRoom::Instance().get_input(outitem->getOutput()->get_param("iid"));
                if (outitem->getOutput()->get_gui_type() == "camera")
                    input = ListeRoom::Instance().get_input(outitem->getOutput()->get_param("iid"));
            }
        }
        else
        {
            input = initem->getInput();
        }

        if (!input)
        {
            QMessageBox::warning(this, tr("Calaos Installer"), tr("You must select an input."));
            return;
        }

        string id = input->get_param("id");
        if (input->get_gui_type() == "audio" ||
            input->get_gui_type() == "camera")
            id = input->get_param("iid");

        Rule *rule = getCurrentRule();
        if (!rule) return;

        Condition *cond = new Condition(COND_STD);
        cond->Add(input);

        cond->get_operator().Add(id, "==");

        if (input->get_gui_type() == "switch" || input->get_gui_type() == "time" ||
            input->get_gui_type() == "timer" || input->get_gui_type() == "scenario" ||
            input->get_gui_type() == "time_range" || input->get_gui_type() == "var_bool")
            cond->get_params().Add(id, "true");
        else if (input->get_gui_type() == "switch3" ||
                 input->get_gui_type() == "switch_long")
            cond->get_params().Add(id, "1");

        rule->AddCondition(cond);

        addItemCondition(cond, true, true);

        setProjectModified(true);
    }
    else if (type == COND_OUTPUT)
    {
        QTreeWidgetItemOutput *initem = dynamic_cast<QTreeWidgetItemOutput *>(ui->tree_home->currentItem());
        if (!initem)
        {
            QMessageBox::warning(this, tr("Calaos Installer"), tr("You must select an output."));
            return;
        }

        IOBase *output = initem->getOutput();
        if (!output) return;

        string id = output->get_param("id");
        if (output->get_gui_type() == "audio" ||
            output->get_gui_type() == "camera")
            id = output->get_param("iid");

        Rule *rule = getCurrentRule();
        if (!rule) return;

        Condition *cond = new Condition(COND_OUTPUT);
        cond->setOutput(output);
        cond->setOutputOper("==");
        cond->setOutputParam("changed");

        rule->AddCondition(cond);

        addItemCondition(cond, true, true);

        setProjectModified(true);
    }
    else if (type == COND_START)
    {
        Rule *rule = getCurrentRule();
        if (!rule) return;

        Condition *cond = new Condition(COND_START);

        rule->AddCondition(cond);

        addItemCondition(cond, true, true);

        setProjectModified(true);
    }
    else if (type == COND_SCRIPT)
    {
        Rule *rule = getCurrentRule();
        if (!rule) return;

        Condition *cond = new Condition(COND_SCRIPT);

        rule->AddCondition(cond);

        addItemCondition(cond, true, true);

        setProjectModified(true);
    }
}

void FormRules::addAction(int type)
{
    if (type == ACTION_STD)
    {
        QTreeWidgetItemOutput *outitem = dynamic_cast<QTreeWidgetItemOutput *>(ui->tree_home->currentItem());
        if (!outitem)
        {
            QMessageBox::warning(this, tr("Calaos Installer"), tr("You must select an output."));
            return;
        }

        IOBase *output = outitem->getOutput();
        if (!output) return;

        string id = output->get_param("id");
        if (output->get_gui_type() == "audio" ||
            output->get_gui_type() == "camera")
            id = output->get_param("oid");

        Rule *rule = getCurrentRule();
        if (!rule) return;

        Action *action = new Action(ACTION_STD);

        action->Add(output);

        if (output->get_gui_type() == "light" || output->get_gui_type() == "light_dimmer" ||
            output->get_gui_type() == "light_rgb" || output->get_gui_type() == "shutter" ||
            output->get_gui_type() == "shutter_smart" || output->get_gui_type() == "var_bool")
            action->get_params().Add(id, "toggle");
        else if (output->get_gui_type() == "scenario" || output->get_gui_type() == "timer")
            action->get_params().Add(id, "true");

        rule->AddAction(action);

        addItemAction(action, true, true);

        setProjectModified(true);
    }
    else if (type == ACTION_MAIL)
    {
        Rule *rule = getCurrentRule();
        if (!rule) return;

        Action *action = new Action(ACTION_MAIL);

        rule->AddAction(action);

        addItemAction(action, true, true);

        setProjectModified(true);
    }
    else if (type == ACTION_SCRIPT)
    {
        Rule *rule = getCurrentRule();
        if (!rule) return;

        Action *action = new Action(ACTION_SCRIPT);

        rule->AddAction(action);

        addItemAction(action, true, true);

        setProjectModified(true);
    }
    else if (type == ACTION_TOUCHSCREEN)
    {
        Rule *rule = getCurrentRule();
        if (!rule) return;

        Action *action = new Action(ACTION_TOUCHSCREEN);

        rule->AddAction(action);

        addItemAction(action, true, true);

        setProjectModified(true);
    }
}

void FormRules::deleteItemCondition()
{
    if (!treeItem_condition) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Calaos Installer"),
                                  tr("Are you sure to delete the item \"%1\"").arg(treeItem_condition->data(0, TwoLineItemDelegate::headerTextRole).toString()),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
        return;

    Rule *rule = getCurrentRule();
    if (!rule) return;

    int num = ui->tree_condition->invisibleRootItem()->indexOfChild(ui->tree_condition->currentItem());

    if (num < 0 || num >= rule->get_size_conds())
        return;

    setProjectModified(true);

    rule->RemoveCondition(num);

    QTreeWidgetItem *item = ui->tree_rules->selectedItems().first();
    ui->tree_rules->setCurrentItem(NULL);
    ui->tree_rules->setCurrentItem(item);
}

void FormRules::deleteItemAction()
{
    if (!treeItem_action) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Calaos Installer"),
                                  tr("Are you sure to delete the item \"%1\"").arg(treeItem_action->data(0, TwoLineItemDelegate::headerTextRole).toString()),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
        return;

    Rule *rule = getCurrentRule();
    if (!rule) return;

    int num = ui->tree_action->invisibleRootItem()->indexOfChild(ui->tree_action->currentItem());

    if (num < 0 || num >= rule->get_size_actions())
        return;

    setProjectModified(true);

    rule->RemoveAction(num);

    QTreeWidgetItem *item = ui->tree_rules->selectedItems().first();
    ui->tree_rules->setCurrentItem(NULL);
    ui->tree_rules->setCurrentItem(item);
}

void FormRules::on_bt_condition_up_clicked()
{
    treeItem_condition = ui->tree_condition->currentItem();
    if (!treeItem_condition) return;

    Rule *rule = getCurrentRule();
    if (!rule) return;

    int num = ui->tree_condition->invisibleRootItem()->indexOfChild(ui->tree_condition->currentItem());

    if (num < 0 || num >= rule->get_size_conds())
        return;

    rule->MoveConditionUp(num);

    on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);

    setProjectModified(true);
}

void FormRules::on_bt_condition_down_clicked()
{
    treeItem_condition = ui->tree_condition->currentItem();
    if (!treeItem_condition) return;

    Rule *rule = getCurrentRule();
    if (!rule) return;

    int num = ui->tree_condition->invisibleRootItem()->indexOfChild(ui->tree_condition->currentItem());

    if (num < 0 || num >= rule->get_size_conds())
        return;

    rule->MoveConditionDown(num);

    on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);

    setProjectModified(true);
}

void FormRules::on_bt_action_up_clicked()
{
    treeItem_action = ui->tree_action->currentItem();
    if (!treeItem_action) return;

    Rule *rule = getCurrentRule();
    if (!rule) return;

    int num = ui->tree_action->invisibleRootItem()->indexOfChild(ui->tree_action->currentItem());

    if (num < 0 || num >= rule->get_size_actions())
        return;

    rule->MoveActionUp(num);

    on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);

    setProjectModified(true);
}

void FormRules::on_bt_action_down_clicked()
{
    treeItem_action = ui->tree_action->currentItem();
    if (!treeItem_action) return;

    Rule *rule = getCurrentRule();
    if (!rule) return;

    int num = ui->tree_action->invisibleRootItem()->indexOfChild(ui->tree_action->currentItem());

    if (num < 0 || num >= rule->get_size_actions())
        return;

    rule->MoveActionDown(num);

    on_tree_rules_currentItemChanged(ui->tree_rules->currentItem(), NULL);

    setProjectModified(true);
}
