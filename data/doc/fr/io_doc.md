
#AVReceiver
Objet AVReceiver pour contrôler l'amplificateur réseau

##Parameters of AVReceiver
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
model | string | yes | Modèle AVReceiver. Pris en charge : pionnier, denon, onkyo, marantz, yamaha
zone | int | no | Zone de l'amplificateur (si pris en charge)
host | string | yes | Adresse IP de l'appareil
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
port | int | no | Port pour la connexion
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Actions of AVReceiver
Name | Description
---- | -----------
custom XXXXXX | Envoyer une commande personnalisée au récepteur (si vous connaissez le protocole) 
 power on | Allumer le récepteur 
 power off | Eteindre le récepteur 
 volume 50 | Régler le volume 
 source X | Changer l'entrée actuelle 
 
#Axis - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!


##Parameters of Axis
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.


#BlinkstickOutputLightRGB
Variateur de lumière RVB avec un Blinkstick


Lumière RVB. Choisir une couleur à définir pour cette lumière.

##Parameters of BlinkstickOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
serial | string | yes | Blinkstick série à contrôler
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
nb_leds | int | yes | Nombres de LEDs à contrôler le blinkstick
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of BlinkstickOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of BlinkstickOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 true | Allumer la lumière 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_green 50 | Mettre le canal vert à X pourcent 
 
##More Infos
* OLA: http://www.blinkstick.com

#Gadspot - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!


##Parameters of Gadspot
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.


#GpioInputSwitch
Entrée GPIO


Interrupteur de base avec états de appuyé/relâché.

##Parameters of GpioInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Mettre à vrai pour inverser le GPIO
gpio | int | yes | ID du GPIO sur la carte
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of GpioInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 

#GpioInputSwitchLongPress
Entrée GPIO


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

##Parameters of GpioInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Mettre à vrai pour inverser le GPIO
gpio | int | yes | ID du GPIO sur la carte
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of GpioInputSwitchLongPress
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 1 | Événement déclenché lors d'un appui court 
 

#GpioInputSwitchTriple
Entrée GPIO


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

##Parameters of GpioInputSwitchTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Mettre à vrai pour inverser le GPIO
gpio | int | yes | ID du GPIO sur la carte
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of GpioInputSwitchTriple
Name | Description
---- | -----------
3 | Événement déclenché lors d'un triple clic 
 changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un double clic 
 1 | Événement déclenché lors d'un simple clic 
 

#GpioOutputShutter
Volet avec 2 GPIOs


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets à impulsions.

##Parameters of GpioOutputShutter
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gpio_down | int | yes | ID du GPIO pour fermer
active_low_up | bool | no | Mettre à vrai pour inverser le GPIO
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
active_low_down | bool | no | Mettre à vrai pour inverser le GPIO
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
gpio_up | int | yes | ID du GPIO pour ouvrir
time | int | yes | Temps en sec pour ouvrir ou fermer le volet
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet

##Conditions of GpioOutputShutter
Name | Description
---- | -----------
true | Événement lorsque le volet est ouvert 
 false | Événement lorsque le volet est fermé 
 changed | Événement sur toute modification de l'état du volet 
 
##Actions of GpioOutputShutter
Name | Description
---- | -----------
set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse down 200 | Fermer le volet pendant X ms 
 up | Ouvrir le volet 
 down | Fermer le volet 
 impulse up 200 | Ouvrir le volet pendant X ms 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 

#GpioOutputShutterSmart
Volet avec 2 GPIOs


Volet intelligent. Cet volet calcule sa position en fonction du temps nécessaire pour s'ouvrir et se fermer. Il est alors possible de régler directement le volet à une position spécifiée.

##Parameters of GpioOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gpio_down | int | yes | ID du GPIO pour fermer
active_low_up | bool | no | Mettre à vrai pour inverser le GPIO
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
gpio_up | int | yes | ID du GPIO pour ouvrir
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
active_low_down | bool | no | Mettre à vrai pour inverser le GPIO
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
time_down | int | yes | Temps en sec pour que le volet soit complètement fermé. Plus il est précis, plus le positionnement sera précis
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
time_up | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est précis, plus le positionnement sera précis
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet

##Conditions of GpioOutputShutterSmart
Name | Description
---- | -----------
true | Événement lorsque le volet est ouvert 
 false | Événement lorsque le volet est fermé 
 changed | Événement sur toute modification de l'état du volet 
 
##Actions of GpioOutputShutterSmart
Name | Description
---- | -----------
set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set 50 | Mettre le volet à la position X en pourcentage 
 down | Fermer le volet 
 up | Ouvrir le volet 
 impulse down 200 | Fermer le volet pendant X ms 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 toggle | Inverser l'état du volet 
 up 5 | Ouvrir le volet de X pourcent 
 calibrate | Commencez l'étalonnage du volet. Cela ouvre complètement le volet et réinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 stop | Arrêter le volet 
 down 5 | Fermer le volet de X pourcent 
 

#GpioOutputSwitch
Lumière avec un GPIO


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

##Parameters of GpioOutputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Mettre à vrai pour inverser le GPIO
gpio | int | yes | ID du GPIO sur la carte
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of GpioOutputSwitch
Name | Description
---- | -----------
true | Événement lorsque la lumière est allumée 
 false | Événement lorsque la lumière est éteinte 
 changed | Événement à chaque changement 
 
##Actions of GpioOutputSwitch
Name | Description
---- | -----------
set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 

#HueOutputLightRGB
Variateur de lumière RVB avec une Philips Hue


Lumière RVB. Choisir une couleur à définir pour cette lumière.

##Parameters of HueOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
id_hue | string | yes | ID unique décrivant la lumière Hue. Cette valeur est renvoyée par Hue Wizard.
api | string | yes | Clé API renvoyée par la passerelle Hue lorsque l'association a été effectuée. Utilisez Hue Wizard dans Calaos_installer pour obtenir cette valeur automatiquement.
host | string | yes | Adresse IP de la passerelle Philips Hue
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of HueOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of HueOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 true | Allumer la lumière 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_green 50 | Mettre le canal vert à X pourcent 
 
##More Infos
* Meet Hue: http://www.meethue.com


#InputTime
Entrée horaire. Un événement est déclenché lorsque l'heure actuelle est égale à l'heure configurée. Une date spécifique peut également être définie.

##Parameters of InputTime
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
day | int | no | Jour pour l'entrée horaire
month | int | no | Mois pour l'entrée horaire
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
sec | int | yes | Seconde pour l'entrée horaire
visible | bool | no | Une entrée horaire ne peut pas être affichée. Toujours faux
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
year | int | no | Année pour l'entrée horaire
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
min | int | yes | Minute pour l'entrée horaire
hour | int | yes | Heure pour l'entrée horaire

##Conditions of InputTime
Name | Description
---- | -----------
changed | Événement lorsque l'heure change 
 false | Événement lorsque l'heure n'est pas atteinte 
 true | Événement lorsque l'heure est atteinte 
 

#InputTimer
Événement lorsque le temps est écoulé

##Parameters of InputTimer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
autorestart | bool | yes | Redémarrage de la minuterie lorsque le temps est expiré
autostart | bool | yes | Démarrage automatique de la minuterie lorsque calaos démarre
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
sec | int | yes | Secondes pour la minuterie
visible | bool | no | Une minuterie ne peut pas être affichée. Toujours faux
msec | int | yes | Milisecondes pour la minuterie
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
min | int | yes | Minutes pour la minuterie
hour | int | yes | Heure pour la minuterie

##Conditions of InputTimer
Name | Description
---- | -----------
change | Événement à chaque changement 
 false | Événement lorsque la minuterie démarre 
 true | Événement lorsque le temps est écoulé 
 
##Actions of InputTimer
Name | Description
---- | -----------
00:00:00:200 | Réinitialiser le temps configuré. Le format est h:m:s:ms 
 stop | Arrêter la minuterie 
 start | Démarrer la minuterie 
 

#InternalBool
Booléen interne. Cet objet est utile pour effectuer une programmation interne dans les règles, comme le maintien d'états booléens ou l'affichage de valeurs booléennes

##Parameters of InternalBool
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
save | bool | no | Enregistrer automatiquement la valeur dans le cache. La valeur sera rechargée lors du démarrage de calaos. Défaut à faux
rw | bool | no | Activer le mode d'édition pour cet objet. Il permet aux utilisateurs de modifier la valeur sur les interfaces. Faux par défaut
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of InternalBool
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Événement lorsque le booléen est à faux 
 true | Événement lorsque le booléen est à vrai 
 
##Actions of InternalBool
Name | Description
---- | -----------
true | Mettre la valeur à vrai 
 false | Mettre la valeur à faux 
 toggle | Inverser la valeur 
 impulse 500 200 500 200 | Impulsion sur la valeur booléenne avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Faites une impulsion sur une valeur booléenne. Mis à vrai pour X ms puis réinitialiser à faux 
 

#InternalInt
Objet de numérique interne. Cet objet est utile pour la programmation interne dans les règles, comme les compteurs, d'affichage des valeurs.

##Parameters of InternalInt
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
save | bool | no | Enregistrer automatiquement la valeur dans le cache. La valeur sera rechargée lors du démarrage de calaos. Défaut à faux
rw | bool | no | Activer le mode d'édition pour cet objet. Il permet aux utilisateurs de modifier la valeur sur les interfaces. Faux par défaut
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of InternalInt
Name | Description
---- | -----------
changed | Événement à chaque changement 
 0 | Événement sur une valeur spécifique 
 
##Actions of InternalInt
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 0 | Définir une valeur spécifique 
 inc | Augmenter la valeur avec le pas définit 
 dec | Diminuer la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 

#InternalString
Chaîne interne. Cet objet est utile pour effectuer une programmation interne dans les règles ou afficher des valeurs de texte sur les interfaces utilisateur.

##Parameters of InternalString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
save | bool | no | Enregistrer automatiquement la valeur dans le cache. La valeur sera rechargée lors du démarrage de calaos. Défaut à faux
rw | bool | no | Activer le mode d'édition pour cet objet. Il permet aux utilisateurs de modifier la valeur sur les interfaces. Faux par défaut
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of InternalString
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une chaine spécifique 
 
##Actions of InternalString
Name | Description
---- | -----------
value | Définir la valeur de la chaine 
 

#KNXInputAnalog
Entrée analogique KNX avec eibnetmux


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

##Parameters of KNXInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of KNXInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXInputSwitch
Interrupteur KNX avec eibnetmux


Interrupteur de base avec états de appuyé/relâché.

##Parameters of KNXInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
eis | int | no | KNX EIS (type de données)
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut

##Conditions of KNXInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXInputSwitchLongPress
Appui long KNX avec eibnetmux


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

##Parameters of KNXInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
eis | int | no | KNX EIS (type de données)
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut

##Conditions of KNXInputSwitchLongPress
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 1 | Événement déclenché lors d'un appui court 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXInputSwitchTriple
Triple clic KNX avec eibnetmux


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

##Parameters of KNXInputSwitchTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
eis | int | no | KNX EIS (type de données)
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut

##Conditions of KNXInputSwitchTriple
Name | Description
---- | -----------
3 | Événement déclenché lors d'un triple clic 
 changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un double clic 
 1 | Événement déclenché lors d'un simple clic 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXInputTemp
Température KNX avec eibnetmux


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

##Parameters of KNXInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of KNXInputTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputAnalog
Sortie analogique KNX avec eibnetmux


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

##Parameters of KNXOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.

##Conditions of KNXOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
##Actions of KNXOutputAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 0 | Définir une valeur spécifique 
 inc | Augmenter la valeur avec le pas définit 
 dec | Diminuer la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputLight
Lumière KNX avec eibnetmux


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

##Parameters of KNXOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
eis | int | no | KNX EIS (type de données)
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut

##Conditions of KNXOutputLight
Name | Description
---- | -----------
true | Événement lorsque la lumière est allumée 
 false | Événement lorsque la lumière est éteinte 
 changed | Événement à chaque changement 
 
##Actions of KNXOutputLight
Name | Description
---- | -----------
set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputLightDimmer
Variateur de lumière KNX avec eibnetmux


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

##Parameters of KNXOutputLightDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
eis | int | no | KNX EIS (type de données)
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut

##Conditions of KNXOutputLightDimmer
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of KNXOutputLightDimmer
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 false | Eteindre la lumière 
 up 5 | Augmenter l'intensité de X pourcent 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputLightRGB
Lumière RVB KNX avec eibnetmux


Lumière RVB. Choisir une couleur à définir pour cette lumière.

##Parameters of KNXOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
listen_knx_group_blue | string | no | Adresse du groupe bleu pour l'écoute de l'état, Ex: x/y/z
listen_knx_group_green | string | no | Adresse du groupe vert pour l'écoute de l'état, Ex: x/y/z
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
knx_group_green | string | yes | Adresse du groupe KNX du canal vert, ex: x/y/z
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
knx_group_blue | string | yes | Adresse du groupe KNX du canal bleu, ex: x/y/z
knx_group_red | string | yes | Adresse du groupe KNX du canal rouge, ex: x/y/z
listen_knx_group_red | string | no | Adresse du groupe rouge pour l'écoute de l'état, Ex: x/y/z

##Conditions of KNXOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of KNXOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 true | Allumer la lumière 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_green 50 | Mettre le canal vert à X pourcent 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputShutter
Volet KNX avec eibnetmux


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets à impulsions.

##Parameters of KNXOutputShutter
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
knx_group_down | string | yes | Adresse du groupe KNX pour fermer, Ex: x/y/z
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host | string | yes | Nom d'hôte de knxd, localhost par défaut
knx_group_up | string | yes | Adresse du groupe KNX pour ouvrir, Ex: x/y/z
time | int | yes | Temps en sec pour ouvrir ou fermer le volet
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet

##Conditions of KNXOutputShutter
Name | Description
---- | -----------
true | Événement lorsque le volet est ouvert 
 false | Événement lorsque le volet est fermé 
 changed | Événement sur toute modification de l'état du volet 
 
##Actions of KNXOutputShutter
Name | Description
---- | -----------
set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse down 200 | Fermer le volet pendant X ms 
 up | Ouvrir le volet 
 down | Fermer le volet 
 impulse up 200 | Ouvrir le volet pendant X ms 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputShutterSmart
Volet KNX avec eibnetmux


Volet intelligent. Cet volet calcule sa position en fonction du temps nécessaire pour s'ouvrir et se fermer. Il est alors possible de régler directement le volet à une position spécifiée.

##Parameters of KNXOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
host | string | yes | Nom d'hôte de knxd, localhost par défaut
knx_group_up | string | yes | Adresse du groupe KNX pour ouvrir, Ex: x/y/z
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
knx_group_down | string | yes | Adresse du groupe KNX pour fermer, Ex: x/y/z
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
time_down | int | yes | Temps en sec pour que le volet soit complètement fermé. Plus il est précis, plus le positionnement sera précis
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
time_up | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est précis, plus le positionnement sera précis
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet

##Conditions of KNXOutputShutterSmart
Name | Description
---- | -----------
true | Événement lorsque le volet est ouvert 
 false | Événement lorsque le volet est fermé 
 changed | Événement sur toute modification de l'état du volet 
 
##Actions of KNXOutputShutterSmart
Name | Description
---- | -----------
set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set 50 | Mettre le volet à la position X en pourcentage 
 down | Fermer le volet 
 up | Ouvrir le volet 
 impulse down 200 | Fermer le volet pendant X ms 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 toggle | Inverser l'état du volet 
 up 5 | Ouvrir le volet de X pourcent 
 calibrate | Commencez l'étalonnage du volet. Cela ouvre complètement le volet et réinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 stop | Arrêter le volet 
 down 5 | Fermer le volet de X pourcent 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#MilightOutputLightRGB
Support des lumières RVB pour les ampoules Limitless/Milight.


Lumière RVB. Choisir une couleur à définir pour cette lumière.

##Parameters of MilightOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zone | int | yes | Zone à contrôler. Chaque passerelle supporte 4 zones.
host | string | yes | Adresse IP de la passerelle wifi Milight
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
port | int | no | Port de la passerelle, 8899 par défaut
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of MilightOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of MilightOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 true | Allumer la lumière 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_green 50 | Mettre le canal vert à X pourcent 
 
##More Infos
* LimitlessLED: http://www.limitlessled.com


#MySensorsInputAnalog
Mesure analogique depuis un noeud MySensors


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

##Parameters of MySensorsInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
gateway | list | yes | Passerelle utilisée, tcp ou série
name | string | yes | Nom de l'entrée/sortie.
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
node_id | string | yes | ID du noeud sur le réseau MySensors
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of MySensorsInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsInputString
Chaine depuis un noeud MySensors

##Parameters of MySensorsInputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host | string | yes | Adresse IP de la passerelle tcp si pertinent
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
node_id | string | yes | ID du noeud sur le réseau MySensors

##More Infos
* MySensors: http://mysensors.org


#MySensorsInputSwitch
Interrupteur depuis un noeud MySensors


Interrupteur de base avec états de appuyé/relâché.

##Parameters of MySensorsInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host | string | yes | Adresse IP de la passerelle tcp si pertinent
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
node_id | string | yes | ID du noeud sur le réseau MySensors

##Conditions of MySensorsInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsInputSwitchLongPress
Appui long depuis un noeud MySensors


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

##Parameters of MySensorsInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host | string | yes | Adresse IP de la passerelle tcp si pertinent
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
node_id | string | yes | ID du noeud sur le réseau MySensors

##Conditions of MySensorsInputSwitchLongPress
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 1 | Événement déclenché lors d'un appui court 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsInputSwitchTriple
Triple clic depuis un noeud MySensors


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

##Parameters of MySensorsInputSwitchTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host | string | yes | Adresse IP de la passerelle tcp si pertinent
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
node_id | string | yes | ID du noeud sur le réseau MySensors

##Conditions of MySensorsInputSwitchTriple
Name | Description
---- | -----------
3 | Événement déclenché lors d'un triple clic 
 changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un double clic 
 1 | Événement déclenché lors d'un simple clic 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsInputTemp
Température depuis un noeud MySensors


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

##Parameters of MySensorsInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
gateway | list | yes | Passerelle utilisée, tcp ou série
name | string | yes | Nom de l'entrée/sortie.
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
node_id | string | yes | ID du noeud sur le réseau MySensors
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of MySensorsInputTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputAnalog
Sortie analogique depuis un noeud MySensors


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

##Parameters of MySensorsOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_DIMMER, voir MySensors.cpp pour plus de valeurs.
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gateway | list | yes | Passerelle utilisée, tcp ou série
name | string | yes | Nom de l'entrée/sortie.
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
node_id | string | yes | ID du noeud sur le réseau MySensors
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.

##Conditions of MySensorsOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
##Actions of MySensorsOutputAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 0 | Définir une valeur spécifique 
 inc | Augmenter la valeur avec le pas définit 
 dec | Diminuer la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputDimmer
Variateur de lumière avec un noeud MySensors


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

##Parameters of MySensorsOutputDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_DIMMER, voir MySensors.cpp pour plus de valeurs.
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host | string | yes | Adresse IP de la passerelle tcp si pertinent
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
node_id | string | yes | ID du noeud sur le réseau MySensors

##Conditions of MySensorsOutputDimmer
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of MySensorsOutputDimmer
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 false | Eteindre la lumière 
 up 5 | Augmenter l'intensité de X pourcent 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputLight
Lumière/Relai avec un noeud MySensors


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

##Parameters of MySensorsOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_LIGHT, voir MySensors.cpp pour plus de valeurs.
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host | string | yes | Adresse IP de la passerelle tcp si pertinent
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
node_id | string | yes | ID du noeud sur le réseau MySensors

##Conditions of MySensorsOutputLight
Name | Description
---- | -----------
true | Événement lorsque la lumière est allumée 
 false | Événement lorsque la lumière est éteinte 
 changed | Événement à chaque changement 
 
##Actions of MySensorsOutputLight
Name | Description
---- | -----------
set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputLightRGB
Variateur de lumière RVB avec un noeud MySensors


Lumière RVB. Choisir une couleur à définir pour cette lumière.

##Parameters of MySensorsOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_DIMMER, voir MySensors.cpp pour plus de valeurs.
sensor_id_blue | string | yes | ID du capteur pour le canal bleu, tel que défini dans votre noeud
sensor_id_green | string | yes | ID du capteur pour le canal vert, tel que défini dans votre noeud
node_id_green | string | yes | ID du noeud pour le canal vert sur le réseau MySensors
sensor_id_red | string | yes | ID du capteur pour le canal rouge, tel que défini dans votre noeud
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
node_id_blue | string | yes | ID du noeud pour le canal bleu sur le réseau MySensors
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host | string | yes | Adresse IP de la passerelle tcp si pertinent
node_id_red | string | yes | ID du noeud pour le canal rouge sur le réseau MySensors

##Conditions of MySensorsOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of MySensorsOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 true | Allumer la lumière 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_green 50 | Mettre le canal vert à X pourcent 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputShutter
Volet avec un noeud MySensors


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets à impulsions.

##Parameters of MySensorsOutputShutter
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_LIGHT, voir MySensors.cpp pour plus de valeurs.
sensor_id_down | string | yes | ID du capteur pour fermer, tel que défini dans votre noeud
node_id_up | string | yes | ID du noeud pour ouvrir sur le réseau MySensors
sensor_id_up | string | yes | ID du capteur pour ouvrir, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
node_id_down | string | yes | ID du noeud pour fermer sur le réseau MySensors
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
time | int | yes | Temps en sec pour ouvrir ou fermer le volet
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet

##Conditions of MySensorsOutputShutter
Name | Description
---- | -----------
true | Événement lorsque le volet est ouvert 
 false | Événement lorsque le volet est fermé 
 changed | Événement sur toute modification de l'état du volet 
 
##Actions of MySensorsOutputShutter
Name | Description
---- | -----------
set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse down 200 | Fermer le volet pendant X ms 
 up | Ouvrir le volet 
 down | Fermer le volet 
 impulse up 200 | Ouvrir le volet pendant X ms 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputShutterSmart
Volet intelligent avec un noeud MySensors


Volet intelligent. Cet volet calcule sa position en fonction du temps nécessaire pour s'ouvrir et se fermer. Il est alors possible de régler directement le volet à une position spécifiée.

##Parameters of MySensorsOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_LIGHT, voir MySensors.cpp pour plus de valeurs.
sensor_id_down | string | yes | ID du capteur pour fermer, tel que défini dans votre noeud
node_id_up | string | yes | ID du noeud pour ouvrir sur le réseau MySensors
sensor_id_up | string | yes | ID du capteur pour ouvrir, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gateway | list | yes | Passerelle utilisée, tcp ou série
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
time_down | int | yes | Temps en sec pour que le volet soit complètement fermé. Plus il est précis, plus le positionnement sera précis
node_id_down | string | yes | ID du noeud pour fermer sur le réseau MySensors
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
time_up | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est précis, plus le positionnement sera précis
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet

##Conditions of MySensorsOutputShutterSmart
Name | Description
---- | -----------
true | Événement lorsque le volet est ouvert 
 false | Événement lorsque le volet est fermé 
 changed | Événement sur toute modification de l'état du volet 
 
##Actions of MySensorsOutputShutterSmart
Name | Description
---- | -----------
set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set 50 | Mettre le volet à la position X en pourcentage 
 down | Fermer le volet 
 up | Ouvrir le volet 
 impulse down 200 | Fermer le volet pendant X ms 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 toggle | Inverser l'état du volet 
 up 5 | Ouvrir le volet de X pourcent 
 calibrate | Commencez l'étalonnage du volet. Cela ouvre complètement le volet et réinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 stop | Arrêter le volet 
 down 5 | Fermer le volet de X pourcent 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputString
Chaine envoyé au noeud MySensors

##Parameters of MySensorsOutputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_VAR1, voir MySensors.cpp pour plus de valeurs.
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host | string | yes | Adresse IP de la passerelle tcp si pertinent
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
node_id | string | yes | ID du noeud sur le réseau MySensors

##More Infos
* MySensors: http://mysensors.org


#OLAOutputLightDimmer
Variateur de lumière DMX avec OLA (Open Lighting Architecture)


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

##Parameters of OLAOutputLightDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
universe | int | yes | Univers OLA à contrôler
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
channel | int | yes | Canal DMX à contrôler
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of OLAOutputLightDimmer
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of OLAOutputLightDimmer
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 false | Eteindre la lumière 
 up 5 | Augmenter l'intensité de X pourcent 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 
##More Infos
* OLA: http://www.openlighting.org


#OLAOutputLightRGB
Le variateur de lumière RVB utilise 3 canaux DMX avec OLA (Open Lighting Architecture)


Lumière RVB. Choisir une couleur à définir pour cette lumière.

##Parameters of OLAOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
channel_red | int | yes | Canal DMX rouge à contrôler
channel_blue | int | yes | Canal DMX bleu à contrôler
channel_green | int | yes | Canal DMX vert à contrôler
universe | int | yes | Univers OLA à contrôler
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of OLAOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of OLAOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 true | Allumer la lumière 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_green 50 | Mettre le canal vert à X pourcent 
 
##More Infos
* OLA: http://www.openlighting.org


#OutputFake
Fausse sortie de test. Ne fait rien. Ne pas utiliser.

##Parameters of OutputFake
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.


#OWTemp
Capteur de tempérarure OneWire DS18B20


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

##Parameters of OWTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
ow_args | string | yes | Paramètre supplémentaire utilisé pour initialiser owfs. Par exemple, -u permet d'utiliser les drivers USB de owfs
ow_id | string | yes | Identifiant du capteur sur le bus OneWire
use_w1 | bool | no | Forcer l'utilisation du pilote kernel w1 au lieu du pilote OneWire
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of OWTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/OneWire


#PingInputSwitch
Entrée basée sur le résultat d'une commande ping. Utile pour détecter la présence d'un hôte sur le réseau.


Interrupteur de base avec états de appuyé/relâché.

##Parameters of PingInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
interval | int | no | Intervalle entre 2 pings en ms. 15 sec par défaut 
timeout | int | no | Délai d'attente en ms pour la réponse du ping
host | string | yes | Adresse IP ou nom de l'hôte pour le ping
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of PingInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | L'hôte est n'en pas en ligne et/ou ne répond pas au ping 
 true | L'hôte est en ligne et répond au ping 
 
#Planet - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!


##Parameters of Planet
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.


#Scenario
Une variable de scénario. Utilisez-la comme un bouton virtuel pour lancer un scénario (liste d'actions)

##Parameters of Scenario
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
auto_scenario | string | no | Utilisation interne uniquement pour les scénarios auto. Lecture seule.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of Scenario
Name | Description
---- | -----------
true | Événement déclenché lors du démarrage du scénario 
 
##Actions of Scenario
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Arrêtez le scénario (uniquement pour les scénarios de boucles spéciales) 
 true | Démarrer le scénario 
 

#Squeezebox
#### Alias: slim
Le lecteur audio Squeezebox permet de contrôler une Squeezebox depuis Calaos

##Parameters of Squeezebox
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
port_web | int | no | Port de l'interface Web, 9000 par défaut
port_cli | int | no | Port du serveur, 9090 par défaut
host | string | yes | Adresse IP du serveur multimédia Logitech
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | ID unique de la squeezebox sur le serveur
visible | bool | yes | Les lecteurs audio ne sont pas affichés dans les pièces
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of Squeezebox
Name | Description
---- | -----------
onvolumechange | Événement lors d'un changement de volume 
 onplaylistchange | Événement lors d'une modification de la liste de lecture 
 onplay | Événement lors de la lecture 
 onpause | Événement lors de la pause 
 onstop | Événement lors de l'arrêt 
 onsongchange | Événement lors de la lecture d'une nouvelle chanson 
 
##Actions of Squeezebox
Name | Description
---- | -----------
add <argument> | Ajouter des pistes à la liste de lecture. <argument> peut être n'importe quel album_id: XX artist_id: XX playlist_id: XX, ... 
 play <argument> | Effacer la liste de lecture et lire argument. <argument> peut être n'importe quel album_id: XX artist_id: XX playlist_id: XX, ... 
 sync <playerid> | Synchronisez ce lecteur avec un autre 
 sleep 10 | Mettre en veille dans X secondes 
 next | Lire la prochaine chanson de la liste de lecture 
 volume down 1 | Diminuer le volume 
 stop | Arrêter la lecture 
 pause | Suspendre la lecture 
 unsync <playerid> | Arrêtez la synchronisation de ce lecteur 
 play | Démarrer la lecture 
 previous | Lire la précédente chanson de la liste de lecture 
 power off | Eteindre le lecteur 
 volume set 50 | Régler le volume 
 volume up 1 | Augmenter le volume 
 power on | Allumer le lecteur 
 
#standard_mjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!


##Parameters of standard_mjpeg
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

#StandardMjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!


##Parameters of StandardMjpeg
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.


#TeleinfoInputAnalog
Mesure analogique extraite des informations Teleinfo.


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

##Parameters of TeleinfoInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
value | list | yes | Toutes ces valeurs sont lues sur l'équipement de Teleinfo en double précision.
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
port | string | yes | Port sur lequel obtenir les informations Teleinfo habituellement un port série comme / dev / ttyS0 ou / dev / ttyAMA0
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of TeleinfoInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

#TimeRange
#### Alias: InPlageHoraire
Représentez un objet de plage de temps. Une plage de temps est vraie si l'heure actuelle est dans l'une des gammes incluses, false sinon. La gamme de temps prend également en charge les jours de semaine et les mois.

##Parameters of TimeRange
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Une plage de temps ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of TimeRange
Name | Description
---- | -----------
changed | Événement déclenché à chaque changement de plage 
 false | Événement déclenché lors de la sortie dans la plage 
 true | Événement déclenché lors de l'entrée dans la plage 
 

#WebInputAnalog
Valeur analogique lue dans un document Web


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

##Parameters of WebInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier téléchargé sera lu et les informations seront extraites du chemin. Par exemple weather[0]/description, essayez de lire la description du premier élément du tableau de l'objet weahter.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier téléchargé est renvoyé en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisée à l'aide du séparateur en tant que délimiteurs. La valeur renvoyée est la valeur à pos dans la ligne fractionnée. Si le séparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

file_type | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
url | string | yes | URL où télécharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of WebInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

#WebInputString
Chaine fournis par un document Web

##Parameters of WebInputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier téléchargé sera lu et les informations seront extraites du chemin. Par exemple weather[0]/description, essayez de lire la description du premier élément du tableau de l'objet weahter.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier téléchargé est renvoyé en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisée à l'aide du séparateur en tant que délimiteurs. La valeur renvoyée est la valeur à pos dans la ligne fractionnée. Si le séparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

url | string | yes | URL où télécharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
file_type | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.


#WebInputTemp
Température lu dans un document Web


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

##Parameters of WebInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier téléchargé sera lu et les informations seront extraites du chemin. Par exemple weather[0]/description, essayez de lire la description du premier élément du tableau de l'objet weahter.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier téléchargé est renvoyé en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisée à l'aide du séparateur en tant que délimiteurs. La valeur renvoyée est la valeur à pos dans la ligne fractionnée. Si le séparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

file_type | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
url | string | yes | URL où télécharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of WebInputTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 

#WebOutputAnalog
Analog output in a web request


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

##Parameters of WebOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier téléchargé sera lu et les informations seront extraites du chemin. Par exemple weather[0]/description, essayez de lire la description du premier élément du tableau de l'objet weahter.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier téléchargé est renvoyé en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisée à l'aide du séparateur en tant que délimiteurs. La valeur renvoyée est la valeur à pos dans la ligne fractionnée. Si le séparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

file_type | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
url | string | yes | URL où télécharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.

##Conditions of WebOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
##Actions of WebOutputAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 0 | Définir une valeur spécifique 
 inc | Augmenter la valeur avec le pas définit 
 dec | Diminuer la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 

#WebOutputLight
Booléen à écrire dans un document Web ou une URL


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

##Parameters of WebOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | yes | Entête HTTP Content-Type utilisé lors de l'envoi du document. Cela dépend du site, mais vous pouvez utiliser application/json ou application/xml comme valeurs correctes.
data | string | yes | Données à publier. Cette valeur peut être nulle, dans ce cas, la valeur est remplacée dans l'URL, sinon la valeur __##VALUE##__ contenue dans les données est remplacée par la valeur à envoyer.
url | string | yes | URL pour poster le document. Le champ data est envoyé en POST si il n'est pas nul. Lorsque aucune donnée n'est fournie, Calaos substitue __##VALUE##__ de l'URL avec la valeur à envoyer. Par exemple, si l'url est http://example.com/api?value=__##VALUE##__ l'url sera :
http://example.com/api?value=20.3
L'URL est encodée avant d'être envoyée.
Si l'URL commence par / ou file:// les données sont écrites dans un fichier local.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of WebOutputLight
Name | Description
---- | -----------
true | Événement lorsque la lumière est allumée 
 false | Événement lorsque la lumière est éteinte 
 changed | Événement à chaque changement 
 
##Actions of WebOutputLight
Name | Description
---- | -----------
set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 

#WebOutputLightRGB
Valeur RVB à écrire dans un document Web ou une URL


Lumière RVB. Choisir une couleur à définir pour cette lumière.

##Parameters of WebOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | yes | Entête HTTP Content-Type utilisé lors de l'envoi du document. Cela dépend du site, mais vous pouvez utiliser application/json ou application/xml comme valeurs correctes.
data | string | yes | Données à publier. Cette valeur peut être nulle, dans ce cas, la valeur est remplacée dans l'URL, sinon la valeur __##VALUE##__ contenue dans les données est remplacée par la valeur à envoyer.
url | string | yes | URL pour poster le document. Le champ data est envoyé en POST si il n'est pas nul. Lorsque aucune donnée n'est fournie, Calaos substitue __##VALUE##__ de l'URL avec la valeur à envoyer. Par exemple, si l'url est http://example.com/api?value=__##VALUE##__ l'url sera :
http://example.com/api?value=20.3
L'URL est encodée avant d'être envoyée.
Si l'URL commence par / ou file:// les données sont écrites dans un fichier local.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
raw_value | bool | no | Valeur RVB au format #RRVVBB. Certaines web api prennent seulement le format RRVVBB. Si Raw_value est à vrai, le # est supprimé. La valeur par défaut est faux.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of WebOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of WebOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 true | Allumer la lumière 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_green 50 | Mettre le canal vert à X pourcent 
 

#WebOutputString
Ecriture d'une chaine dans un document web ou une URL.

##Parameters of WebOutputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | yes | Entête HTTP Content-Type utilisé lors de l'envoi du document. Cela dépend du site, mais vous pouvez utiliser application/json ou application/xml comme valeurs correctes.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
file_type | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
url | string | yes | URL pour poster le document. Le champ data est envoyé en POST si il n'est pas nul. Lorsque aucune donnée n'est fournie, Calaos substitue __##VALUE##__ de l'URL avec la valeur à envoyer. Par exemple, si l'url est http://example.com/api?value=__##VALUE##__ l'url sera :
http://example.com/api?value=20.3
L'URL est encodée avant d'être envoyée.
Si l'URL commence par / ou file:// les données sont écrites dans un fichier local.
data | string | yes | Données à publier. Cette valeur peut être nulle, dans ce cas, la valeur est remplacée dans l'URL, sinon la valeur __##VALUE##__ contenue dans les données est remplacée par la valeur à envoyer.
path | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier téléchargé sera lu et les informations seront extraites du chemin. Par exemple weather[0]/description, essayez de lire la description du premier élément du tableau de l'objet weahter.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier téléchargé est renvoyé en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisée à l'aide du séparateur en tant que délimiteurs. La valeur renvoyée est la valeur à pos dans la ligne fractionnée. Si le séparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3



#WIAnalog
#### Alias: WagoInputAnalog
Mesure analogique avec un module Wago (comme 0-10V, 4-20mA, ...)


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

##Parameters of WIAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Adresse IP de l'automate Wago
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
var | int | yes | Adresse PLC du capteur d'entrée
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
port | int | no | Port de l'automate Wage, 502 par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of WIAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/entree_analog


#WIDigitalBP
#### Alias: WIDigital, WagoInputSwitch
Interrupteur avec entrée numérique Modules Wago (comme 750-1405, ...)


Interrupteur de base avec états de appuyé/relâché.

##Parameters of WIDigitalBP
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Adresse IP de l'automate Wago
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
var | int | yes | Adresse PLC de l'entrée numérique
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
port | int | no | Port de l'automate Wage, 502 par défaut
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of WIDigitalBP
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


#WIDigitalLong
#### Alias: WagoInputSwitchLongPress
Appui long avec les modules Wago d'entrée numérique (comme 750-1405, ...)


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

##Parameters of WIDigitalLong
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Adresse IP de l'automate Wago
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
var | int | yes | Adresse PLC de l'entrée numérique
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
port | int | no | Port de l'automate Wage, 502 par défaut
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of WIDigitalLong
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 1 | Événement déclenché lors d'un appui court 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


#WIDigitalTriple
#### Alias: WagoInputSwitchTriple
Triple clic avec les modules Wago d'entrée numérique (comme 750-1405, ...)


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

##Parameters of WIDigitalTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Adresse IP de l'automate Wago
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
var | int | yes | Adresse PLC de l'entrée numérique
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
port | int | no | Port de l'automate Wage, 502 par défaut
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of WIDigitalTriple
Name | Description
---- | -----------
3 | Événement déclenché lors d'un triple clic 
 changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un double clic 
 1 | Événement déclenché lors d'un simple clic 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


#WITemp
#### Alias: WagoInputTemp
Mesure de la température avec module de température Wago (comme 750-460)


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

##Parameters of WITemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Adresse IP de l'automate Wago
var | int | yes | Adresse PLC du capteur d'entrée
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
port | int | no | Port de l'automate Wage, 502 par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of WITemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-460


#WOAnalog
#### Alias: WagoOutputAnalog
Sortie analogique avec module Wago (comme 0-10V, 4-20mA, ...)


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

##Parameters of WOAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Adresse IP de l'automate Wago
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
port | int | no | Port de l'automate Wage, 502 par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
var | int | yes | Adresse de l'automate de la sortie
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.

##Conditions of WOAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
##Actions of WOAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 0 | Définir une valeur spécifique 
 inc | Augmenter la valeur avec le pas définit 
 dec | Diminuer la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/sortie_analog


#WODali
#### Alias: WagoOutputDimmer
Lumière utilisant DALI ou DMX. Pour DALI, vous avez besoin d'un module wago 750-641. Pour DMX, un périphérique DMX4ALL-LAN connecté au Wago PLC.


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

##Parameters of WODali
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
fade_time | int | no | Durée du fondu DALI (entre 1 et 10)
group | int | no | Réglez sur 1 pour une adresse de groupe DALI, sinon mettre 0
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
address | int | yes | Adresse de l'appareil. Les adresses DALI se situent entre 1 et 64. Pour les DMX, les adresses commencent à 100. Donc, pour le périphérique DMX 5, l'adresse doit être 105.
host | string | yes | Adresse IP de l'automate Wago
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | int | no | Port de l'automate Wage, 502 par défaut
line | int | no | Ligne de bus DALI, habituellement 1

##Conditions of WODali
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of WODali
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 false | Eteindre la lumière 
 up 5 | Augmenter l'intensité de X pourcent 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-641
* Calaos Wiki: http://calaos.fr/wiki/fr/dmx-lan


#WODaliRVB
#### Alias: WagoOutputDimmerRGB
Lumière RVB utilisant DALI ou DMX. Pour fonctionner, vous avez besoin de 3 canaux DALI/DMX. Pour DALI, vous avez besoin d'un module wago 750-641. Pour DMX, un périphérique DMX4ALL-LAN connecté au Wago PLC.


Lumière RVB. Choisir une couleur à définir pour cette lumière.

##Parameters of WODaliRVB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gfade_time | int | no | Durée du fondu pour le canal vert DALI (entre 1 et 10)
gline | int | no | Ligne de bus DALI pour canal vert, habituellement 1
bgroup | int | no | Réglez sur 1 si l'adresse du canal bleu est une adresse de groupe DALI, sinon mettre 0
rgroup | int | no | Réglez sur 1 si l'adresse du canal rouge est une adresse de groupe DALI,  sinon mettre 0
bfade_time | int | no | Durée du fondu pour le canal bleu DALI (entre 1 et 10)
bline | int | no | Ligne de bus DALI pour canal bleu, habituellement 1
gaddress | int | yes | Adresse pour le canal vert. Les adresses DALI se situent entre 1 et 64. Pour les DMX, les adresses commencent à 100. Donc, pour le périphérique DMX 5, l'adresse doit être 105.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
ggroup | int | no | Réglez sur 1 si l'adresse du canal vert est une adresse de groupe DALI, sinon mettre 0
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | int | no | Port de l'automate Wage, 502 par défaut
baddress | int | yes | Adresse pour le canal bleu. Les adresses DALI se situent entre 1 et 64. Pour les DMX, les adresses commencent à 100. Donc, pour le périphérique DMX 5, l'adresse doit être 105.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
raddress | int | yes | Adresse pour le canal rouge. Les adresses DALI se situent entre 1 et 64. Pour les DMX, les adresses commencent à 100. Donc, pour le périphérique DMX 5, l'adresse doit être 105.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
rfade_time | int | no | Durée du fondu pour le canal rouge DALI (entre 1 et 10)
rline | int | no | Ligne de bus DALI pour canal rouge, habituellement 1
host | string | yes | Adresse IP de l'automate Wago

##Conditions of WODaliRVB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of WODaliRVB
Name | Description
---- | -----------
down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 true | Allumer la lumière 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_green 50 | Mettre le canal vert à X pourcent 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-641
* Calaos Wiki: http://calaos.fr/wiki/fr/dmx-lan


#WODigital
#### Alias: WagoOutputLight
Contrôle d'une lumière ou d'un relais à l'aide des modules de sortie numérique wago (comme 750-1504, ...)


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

##Parameters of WODigital
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | int | no | Port de l'automate Wage, 502 par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
var | int | yes | Adresse de l'automate de la sortie
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
knx | bool | no | Mettre à true si la sortie est un périphérique KNX (uniquement pour 750-849 avec le module KNX / TP1)
host | string | yes | Adresse IP de l'automate Wago
wago_841 | bool | yes | Doit être faux pour le PLC 750-842, sinon vrai

##Conditions of WODigital
Name | Description
---- | -----------
true | Événement lorsque la lumière est allumée 
 false | Événement lorsque la lumière est éteinte 
 changed | Événement à chaque changement 
 
##Actions of WODigital
Name | Description
---- | -----------
set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


#WOLOutputBool
Objet réveil réseau. Envoye un paquet wake-on-lan à un périphérique sur le réseau.

##Parameters of WOLOutputBool
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
interval | int | no | Intervalle entre 2 pings en ms. 15 sec par défaut 
address | string | yes | Adresse MAC de l'hôte à réveiller
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Actions of WOLOutputBool
Name | Description
---- | -----------
true | Envoie un ordre de réveil au périphérique configuré 
 

#WOVolet
#### Alias: WagoOutputShutter
Volet utilisant des modules de sortie numérique wago (comme 750-1504, ...)


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets à impulsions.

##Parameters of WOVolet
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
var_down | int | yes | Sortie numérique sur l'automate pour fermer le volet
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
var_up | int | yes | Sortie numérique sur l'automate pour ouvrir le volet
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
wago_841 | bool | yes | Doit être faux pour le PLC 750-842, sinon vrai
time | int | yes | Temps en sec pour ouvrir ou fermer le volet
port | int | no | Port de l'automate Wage, 502 par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
knx | bool | no | Mettre à true si la sortie est un périphérique KNX (uniquement pour 750-849 avec le module KNX / TP1)
host | string | yes | Adresse IP de l'automate Wago
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet

##Conditions of WOVolet
Name | Description
---- | -----------
true | Événement lorsque le volet est ouvert 
 false | Événement lorsque le volet est fermé 
 changed | Événement sur toute modification de l'état du volet 
 
##Actions of WOVolet
Name | Description
---- | -----------
set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse down 200 | Fermer le volet pendant X ms 
 up | Ouvrir le volet 
 down | Fermer le volet 
 impulse up 200 | Ouvrir le volet pendant X ms 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


#WOVoletSmart
#### Alias: WagoOutputShutterSmart
Volet intelligent utilisant des modules de sortie numérique wago (comme 750-1504, ...)


Volet intelligent. Cet volet calcule sa position en fonction du temps nécessaire pour s'ouvrir et se fermer. Il est alors possible de régler directement le volet à une position spécifiée.

##Parameters of WOVoletSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
wago_841 | bool | yes | Doit être faux pour le PLC 750-842, sinon vrai
var_down | int | yes | Sortie numérique sur l'automate pour fermer le volet
knx | bool | no | Mettre à true si la sortie est un périphérique KNX (uniquement pour 750-849 avec le module KNX / TP1)
host | string | yes | Adresse IP de l'automate Wago
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
var_up | int | yes | Sortie numérique sur l'automate pour ouvrir le volet
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
time_down | int | yes | Temps en sec pour que le volet soit complètement fermé. Plus il est précis, plus le positionnement sera précis
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
time_up | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est précis, plus le positionnement sera précis
port | int | no | Port de l'automate Wage, 502 par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet

##Conditions of WOVoletSmart
Name | Description
---- | -----------
true | Événement lorsque le volet est ouvert 
 false | Événement lorsque le volet est fermé 
 changed | Événement sur toute modification de l'état du volet 
 
##Actions of WOVoletSmart
Name | Description
---- | -----------
set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set 50 | Mettre le volet à la position X en pourcentage 
 down | Fermer le volet 
 up | Ouvrir le volet 
 impulse down 200 | Fermer le volet pendant X ms 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 toggle | Inverser l'état du volet 
 up 5 | Ouvrir le volet de X pourcent 
 calibrate | Commencez l'étalonnage du volet. Cela ouvre complètement le volet et réinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 stop | Arrêter le volet 
 down 5 | Fermer le volet de X pourcent 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


#X10Output
Variateur de lumière X10.


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

##Parameters of X10Output
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
code | string | yes | House code de la lumière X10
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of X10Output
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
##Actions of X10Output
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 false | Eteindre la lumière 
 up 5 | Augmenter l'intensité de X pourcent 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 

#xPLInputAnalog
Capteur xPL analogique


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

##Parameters of xPLInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
sensor | string | yes | ID du capteur sur le réseau xPL
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of xPLInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

#xPLInputAnalog
Capteur xPL chaine

##Parameters of xPLInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
sensor | string | yes | ID du capteur sur le réseau xPL
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.


#xPLInputSwitch
Entrée xPL


Interrupteur de base avec états de appuyé/relâché.

##Parameters of xPLInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
sensor | string | yes | ID du capteur sur le réseau xPL
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of xPLInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 

#xPLInputTemp
Capteur de température xPL


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

##Parameters of xPLInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
sensor | string | yes | ID du capteur sur le réseau xPL
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of xPLInputTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 

#xPLOutputAnalog
Sortie analogique pilotée par le protocole xPL


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

##Parameters of xPLOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
actuator | string | yes | ID de l'actionneur sur le réseau xPL
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.

##Conditions of xPLOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
##Actions of xPLOutputAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 0 | Définir une valeur spécifique 
 inc | Augmenter la valeur avec le pas définit 
 dec | Diminuer la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 

#xPLOutputAnalog
Sortie analogique pilotée par le protocole xPL

##Parameters of xPLOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
actuator | string | yes | ID de l'actionneur sur le réseau xPL
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.


#xPLOutputSwitch
Lumière/relai piloté par le protocole xPL


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

##Parameters of xPLOutputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
actuator | string | yes | ID de l'actionneur sur le réseau xPL
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
name | string | yes | Nom de l'entrée/sortie.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.

##Conditions of xPLOutputSwitch
Name | Description
---- | -----------
true | Événement lorsque la lumière est allumée 
 false | Événement lorsque la lumière est éteinte 
 changed | Événement à chaque changement 
 
##Actions of xPLOutputSwitch
Name | Description
---- | -----------
set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 

#ZibaseAnalogIn
Entrée analogique Zibase. Cet objet peut lire une valeur à partir de périphériques tels que les capteurs de consommation d'énergie, les capteurs de luminosité, ...


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

##Parameters of ZibaseAnalogIn
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_sensor | list | yes | Type of sensor
host | string | yes | Adresse IP de la Zibase
gui_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
port | int | no | Port de la Zibase, 17100 par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
zibase_id | string | yes | ID du périphérie Zibase (ABC)
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of ZibaseAnalogIn
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

#ZibaseDigitalIn
Entrée numérique Zibase. Cet objet agit comme un commutateur


Interrupteur de base avec états de appuyé/relâché.

##Parameters of ZibaseDigitalIn
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_sensor | list | yes | Type of sensor
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | int | no | Port de la Zibase, 17100 par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
zibase_id | string | yes | ID du premier périphérie Zibase (ABC)
zibase_id2 | string | yes | ID du deuxième périphérie Zibase (ABC)
host | string | yes | Adresse IP de la Zibase

##Conditions of ZibaseDigitalIn
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 

#ZibaseDigitalOut
Sortie numérique Zibase. Cet objet contrôle les périphériques Zibase


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

##Parameters of ZibaseDigitalOut
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
nbburst | int | no | Nombre d'impulsion à envoyer
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
name | string | yes | Nom de l'entrée/sortie.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
port | int | no | Port de la Zibase, 17100 par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
zibase_id | string | yes | ID du périphérie Zibase (ABC)
host | string | yes | Adresse IP de la Zibase
protocol | list | yes | Protocol to use

##Conditions of ZibaseDigitalOut
Name | Description
---- | -----------
true | Événement lorsque la lumière est allumée 
 false | Événement lorsque la lumière est éteinte 
 changed | Événement à chaque changement 
 
##Actions of ZibaseDigitalOut
Name | Description
---- | -----------
set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 true | Allumer la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 toggle | Inverser l'état de la lumière 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 

#ZibaseTemp
Capteur de température Zibase


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

##Parameters of ZibaseTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_sensor | list | yes | Type of sensor
host | string | yes | Adresse IP de la Zibase
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
port | int | no | Port de la Zibase, 17100 par défaut
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
zibase_id | string | yes | ID du périphérie Zibase (ABC)
frequency | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0

##Conditions of ZibaseTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 
