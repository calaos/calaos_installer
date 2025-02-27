
# AVReceiver
Objet AVReceiver pour contrôler l'amplificateur réseau

## Parameters of AVReceiver
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
model | string | yes | Modèle AVReceiver. Pris en charge : pionnier, denon, onkyo, marantz, yamaha
zone | int | no | Zone de l'amplificateur (si pris en charge)
host | string | yes | Adresse IP de l'appareil
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
port | int | no | Port pour la connexion
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Actions of AVReceiver
Name | Description
---- | -----------
custom XXXXXX | Envoyer une commande personnalisée au récepteur (si vous connaissez le protocole) 
 source X | Changer l'entrée actuelle 
 volume 50 | Régler le volume 
 power off | Eteindre le récepteur 
 power on | Allumer le récepteur 
 
# Axis - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Encodeur/Camera IP Axis. La caméra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of Axis
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
resolution | string | no | Résolution à utiliser
name | string | yes | Nom de l'entrée/sortie.
tilt_framesize | string | no | 
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
rotate | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
pan_framesize | string | no | 
model | string | yes | Modèle de caméra/Canal à utiliser
width | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.
ptz | bool | no | Activer si la caméra supporte le PTZ
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
zoom_step | string | no | 

# Foscam - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Foscam IP Camera/Encoder. Camera can be viewed directly inside calaos and used in rules.

## Parameters of Foscam
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
password | string | yes | Mot de passe pour l'utilisateur
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | IP Address
rotate | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
port | string | yes | Port number
width | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.
ptz | bool | no | Activer si la caméra supporte le PTZ
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
zoom_step | string | no | 
username | string | yes | Nom d'utilisateur pour accéder à la caméra

# Gadspot - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Camera IP Gadspot. La caméra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of Gadspot
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
rotate | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
width | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server


# GpioInputSwitch
Entrée GPIO


Interrupteur de base avec états de appuyé/relâché.

## Parameters of GpioInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gpio | int | yes | ID du GPIO sur la carte
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
active_low | bool | no | Mettre à vrai pour inverser le GPIO
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of GpioInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 

# GpioInputSwitchLongPress
Entrée GPIO


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

## Parameters of GpioInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gpio | int | yes | ID du GPIO sur la carte
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
active_low | bool | no | Mettre à vrai pour inverser le GPIO
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of GpioInputSwitchLongPress
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 1 | Événement déclenché lors d'un appui court 
 

# GpioInputSwitchTriple
Entrée GPIO


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

## Parameters of GpioInputSwitchTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gpio | int | yes | ID du GPIO sur la carte
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
active_low | bool | no | Mettre à vrai pour inverser le GPIO
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of GpioInputSwitchTriple
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un double clic 
 3 | Événement déclenché lors d'un triple clic 
 1 | Événement déclenché lors d'un simple clic 
 

# GpioOutputShutter
Volet avec 2 GPIOs


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets à impulsions.

## Parameters of GpioOutputShutter
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low_up | bool | no | Mettre à vrai pour inverser le GPIO
active_low_down | bool | no | Mettre à vrai pour inverser le GPIO
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
gpio_down | int | yes | ID du GPIO pour fermer
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet
time | int | yes | Temps en sec pour ouvrir ou fermer le volet
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
gpio_up | int | yes | ID du GPIO pour ouvrir

## Conditions of GpioOutputShutter
Name | Description
---- | -----------
false | Événement lorsque le volet est fermé 
 true | Événement lorsque le volet est ouvert 
 changed | Événement sur toute modification de l'état du volet 
 
## Actions of GpioOutputShutter
Name | Description
---- | -----------
impulse up 200 | Ouvrir le volet pendant X ms 
 set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse down 200 | Fermer le volet pendant X ms 
 toggle | Inverser l'état du volet 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 stop | Arrêter le volet 
 down | Fermer le volet 
 up | Ouvrir le volet 
 

# GpioOutputShutterSmart
Volet avec 2 GPIOs


Volet intelligent. Cet volet calcule sa position en fonction du temps nécessaire pour s'ouvrir et se fermer. Il est alors possible de régler directement le volet à une position spécifiée.

## Parameters of GpioOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low_up | bool | no | Mettre à vrai pour inverser le GPIO
gpio_down | int | yes | ID du GPIO pour fermer
active_low_down | bool | no | Mettre à vrai pour inverser le GPIO
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
time_up | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est précis, plus le positionnement sera précis
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
time_down | int | yes | Temps en sec pour que le volet soit complètement fermé. Plus il est précis, plus le positionnement sera précis
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
gpio_up | int | yes | ID du GPIO pour ouvrir

## Conditions of GpioOutputShutterSmart
Name | Description
---- | -----------
false | Événement lorsque le volet est fermé 
 true | Événement lorsque le volet est ouvert 
 changed | Événement sur toute modification de l'état du volet 
 
## Actions of GpioOutputShutterSmart
Name | Description
---- | -----------
up 5 | Ouvrir le volet de X pourcent 
 set 50 | Mettre le volet à la position X en pourcentage 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 down 5 | Fermer le volet de X pourcent 
 impulse down 200 | Fermer le volet pendant X ms 
 toggle | Inverser l'état du volet 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 calibrate | Commencez l'étalonnage du volet. Cela ouvre complètement le volet et réinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 stop | Arrêter le volet 
 down | Fermer le volet 
 up | Ouvrir le volet 
 

# GpioOutputSwitch
Lumière avec un GPIO


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of GpioOutputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gpio | int | yes | ID du GPIO sur la carte
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
active_low | bool | no | Mettre à vrai pour inverser le GPIO
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of GpioOutputSwitch
Name | Description
---- | -----------
false | Événement lorsque la lumière est éteinte 
 true | Événement lorsque la lumière est allumée 
 changed | Événement à chaque changement 
 
## Actions of GpioOutputSwitch
Name | Description
---- | -----------
impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 true | Allumer la lumière 
 

# HueOutputLightRGB
Variateur de lumière RVB avec une Philips Hue


Lumière RVB. Choisir une couleur à définir pour cette lumière.

## Parameters of HueOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
api | string | yes | Clé API renvoyée par la passerelle Hue lorsque l'association a été effectuée. Utilisez Hue Wizard dans Calaos_installer pour obtenir cette valeur automatiquement.
host | string | yes | Adresse IP de la passerelle Philips Hue
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
id_hue | string | yes | ID unique décrivant la lumière Hue. Cette valeur est renvoyée par Hue Wizard.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of HueOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of HueOutputLightRGB
Name | Description
---- | -----------
down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_green 50 | Mettre le canal vert à X pourcent 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 
## More Infos
* Meet Hue: http://www.meethue.com


# InputTime
Entrée horaire. Un événement est déclenché lorsque l'heure actuelle est égale à l'heure configurée. Une date spécifique peut également être définie.

## Parameters of InputTime
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Une entrée horaire ne peut pas être affichée. Toujours faux
year | int | no | Année pour l'entrée horaire
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
min | int | yes | Minute pour l'entrée horaire
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
day | int | no | Jour pour l'entrée horaire
hour | int | yes | Heure pour l'entrée horaire
sec | int | yes | Seconde pour l'entrée horaire
name | string | yes | Nom de l'entrée/sortie.
month | int | no | Mois pour l'entrée horaire

## Conditions of InputTime
Name | Description
---- | -----------
changed | Événement lorsque l'heure change 
 false | Événement lorsque l'heure n'est pas atteinte 
 true | Événement lorsque l'heure est atteinte 
 

# InputTimer
Événement lorsque le temps est écoulé

## Parameters of InputTimer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
autorestart | bool | yes | Redémarrage de la minuterie lorsque le temps est expiré
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Une minuterie ne peut pas être affichée. Toujours faux
msec | int | yes | Milisecondes pour la minuterie
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
min | int | yes | Minutes pour la minuterie
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
hour | int | yes | Heure pour la minuterie
autostart | bool | yes | Démarrage automatique de la minuterie lorsque calaos démarre
sec | int | yes | Secondes pour la minuterie

## Conditions of InputTimer
Name | Description
---- | -----------
change | Événement à chaque changement 
 false | Événement lorsque la minuterie démarre 
 true | Événement lorsque le temps est écoulé 
 
## Actions of InputTimer
Name | Description
---- | -----------
00:00:00:200 | Réinitialiser le temps configuré. Le format est h:m:s:ms 
 stop | Arrêter la minuterie 
 start | Démarrer la minuterie 
 

# InternalBool
Booléen interne. Cet objet est utile pour effectuer une programmation interne dans les règles, comme le maintien d'états booléens ou l'affichage de valeurs booléennes

## Parameters of InternalBool
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
rw | bool | no | Activer le mode d'édition pour cet objet. Il permet aux utilisateurs de modifier la valeur sur les interfaces. Faux par défaut
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
save | bool | no | Enregistrer automatiquement la valeur dans le cache. La valeur sera rechargée lors du démarrage de calaos. Défaut à faux
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of InternalBool
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Événement lorsque le booléen est à faux 
 true | Événement lorsque le booléen est à vrai 
 
## Actions of InternalBool
Name | Description
---- | -----------
impulse 200 | Faites une impulsion sur une valeur booléenne. Mis à vrai pour X ms puis réinitialiser à faux 
 impulse 500 200 500 200 | Impulsion sur la valeur booléenne avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 toggle | Inverser la valeur 
 false | Mettre la valeur à faux 
 true | Mettre la valeur à vrai 
 

# InternalInt
Objet de numérique interne. Cet objet est utile pour la programmation interne dans les règles, comme les compteurs, d'affichage des valeurs.

## Parameters of InternalInt
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
rw | bool | no | Activer le mode d'édition pour cet objet. Il permet aux utilisateurs de modifier la valeur sur les interfaces. Faux par défaut
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
save | bool | no | Enregistrer automatiquement la valeur dans le cache. La valeur sera rechargée lors du démarrage de calaos. Défaut à faux
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of InternalInt
Name | Description
---- | -----------
changed | Événement à chaque changement 
 0 | Événement sur une valeur spécifique 
 
## Actions of InternalInt
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 inc 1 | Augmenter valeur par valeur 
 dec | Diminuer la valeur avec le pas définit 
 inc | Augmenter la valeur avec le pas définit 
 0 | Définir une valeur spécifique 
 

# InternalString
Chaîne interne. Cet objet est utile pour effectuer une programmation interne dans les règles ou afficher des valeurs de texte sur les interfaces utilisateur.

## Parameters of InternalString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
rw | bool | no | Activer le mode d'édition pour cet objet. Il permet aux utilisateurs de modifier la valeur sur les interfaces. Faux par défaut
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
save | bool | no | Enregistrer automatiquement la valeur dans le cache. La valeur sera rechargée lors du démarrage de calaos. Défaut à faux
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of InternalString
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une chaine spécifique 
 
## Actions of InternalString
Name | Description
---- | -----------
value | Définir la valeur de la chaine 
 

# KNXInputAnalog
Entrée analogique KNX avec eibnetmux


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

## Parameters of KNXInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
host | string | yes | Nom d'hôte de knxd, localhost par défaut
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of KNXInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputSwitch
Interrupteur KNX avec eibnetmux


Interrupteur de base avec états de appuyé/relâché.

## Parameters of KNXInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
eis | int | no | KNX EIS (type de données)
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of KNXInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputSwitchLongPress
Appui long KNX avec eibnetmux


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

## Parameters of KNXInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
eis | int | no | KNX EIS (type de données)
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of KNXInputSwitchLongPress
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 1 | Événement déclenché lors d'un appui court 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputSwitchTriple
Triple clic KNX avec eibnetmux


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

## Parameters of KNXInputSwitchTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
eis | int | no | KNX EIS (type de données)
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of KNXInputSwitchTriple
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un double clic 
 3 | Événement déclenché lors d'un triple clic 
 1 | Événement déclenché lors d'un simple clic 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputTemp
Température KNX avec eibnetmux


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

## Parameters of KNXInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
host | string | yes | Nom d'hôte de knxd, localhost par défaut
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of KNXInputTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputAnalog
Sortie analogique KNX avec eibnetmux


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

## Parameters of KNXOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.

## Conditions of KNXOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## Actions of KNXOutputAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 inc 1 | Augmenter valeur par valeur 
 dec | Diminuer la valeur avec le pas définit 
 inc | Augmenter la valeur avec le pas définit 
 0 | Définir une valeur spécifique 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputLight
Lumière KNX avec eibnetmux


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of KNXOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
eis | int | no | KNX EIS (type de données)
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut

## Conditions of KNXOutputLight
Name | Description
---- | -----------
false | Événement lorsque la lumière est éteinte 
 true | Événement lorsque la lumière est allumée 
 changed | Événement à chaque changement 
 
## Actions of KNXOutputLight
Name | Description
---- | -----------
impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 true | Allumer la lumière 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputLightDimmer
Variateur de lumière KNX avec eibnetmux


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

## Parameters of KNXOutputLightDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
eis | int | no | KNX EIS (type de données)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of KNXOutputLightDimmer
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of KNXOutputLightDimmer
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 up 5 | Augmenter l'intensité de X pourcent 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputLightRGB
Lumière RVB KNX avec eibnetmux


Lumière RVB. Choisir une couleur à définir pour cette lumière.

## Parameters of KNXOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
listen_knx_group_blue | string | no | Adresse du groupe bleu pour l'écoute de l'état, Ex: x/y/z
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
listen_knx_group_green | string | no | Adresse du groupe vert pour l'écoute de l'état, Ex: x/y/z
knx_group_red | string | yes | Adresse du groupe KNX du canal rouge, ex: x/y/z
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
knx_group_green | string | yes | Adresse du groupe KNX du canal vert, ex: x/y/z
listen_knx_group_red | string | no | Adresse du groupe rouge pour l'écoute de l'état, Ex: x/y/z
knx_group_blue | string | yes | Adresse du groupe KNX du canal bleu, ex: x/y/z

## Conditions of KNXOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of KNXOutputLightRGB
Name | Description
---- | -----------
down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_green 50 | Mettre le canal vert à X pourcent 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputShutter
Volet KNX avec eibnetmux


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets à impulsions.

## Parameters of KNXOutputShutter
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
eis | int | no | KNX EIS (type de données)
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
time | int | yes | Temps en sec pour ouvrir ou fermer le volet
knx_group_down | string | yes | Adresse du groupe KNX pour fermer, Ex: x/y/z
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
knx_group_up | string | yes | Adresse du groupe KNX pour ouvrir, Ex: x/y/z

## Conditions of KNXOutputShutter
Name | Description
---- | -----------
false | Événement lorsque le volet est fermé 
 true | Événement lorsque le volet est ouvert 
 changed | Événement sur toute modification de l'état du volet 
 
## Actions of KNXOutputShutter
Name | Description
---- | -----------
impulse up 200 | Ouvrir le volet pendant X ms 
 set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse down 200 | Fermer le volet pendant X ms 
 toggle | Inverser l'état du volet 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 stop | Arrêter le volet 
 down | Fermer le volet 
 up | Ouvrir le volet 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputShutterSmart
Volet KNX avec eibnetmux


Volet intelligent. Cet volet calcule sa position en fonction du temps nécessaire pour s'ouvrir et se fermer. Il est alors possible de régler directement le volet à une position spécifiée.

## Parameters of KNXOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Nom d'hôte de knxd, localhost par défaut
read_at_start | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
eis | int | no | KNX EIS (type de données)
listen_knx_group | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
knx_group_down | string | yes | Adresse du groupe KNX pour fermer, Ex: x/y/z
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
knx_group | string | yes | Adresse de groupe KNX, Ex: x/y/z
time_up | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est précis, plus le positionnement sera précis
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
time_down | int | yes | Temps en sec pour que le volet soit complètement fermé. Plus il est précis, plus le positionnement sera précis
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
knx_group_up | string | yes | Adresse du groupe KNX pour ouvrir, Ex: x/y/z

## Conditions of KNXOutputShutterSmart
Name | Description
---- | -----------
false | Événement lorsque le volet est fermé 
 true | Événement lorsque le volet est ouvert 
 changed | Événement sur toute modification de l'état du volet 
 
## Actions of KNXOutputShutterSmart
Name | Description
---- | -----------
up 5 | Ouvrir le volet de X pourcent 
 set 50 | Mettre le volet à la position X en pourcentage 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 down 5 | Fermer le volet de X pourcent 
 impulse down 200 | Fermer le volet pendant X ms 
 toggle | Inverser l'état du volet 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 calibrate | Commencez l'étalonnage du volet. Cela ouvre complètement le volet et réinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 stop | Arrêter le volet 
 down | Fermer le volet 
 up | Ouvrir le volet 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# MilightOutputLightRGB
Support des lumières RVB pour les ampoules Limitless/Milight.


Lumière RVB. Choisir une couleur à définir pour cette lumière.

## Parameters of MilightOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zone | int | yes | Zone à contrôler. Chaque passerelle supporte 4 zones.
host | string | yes | Adresse IP de la passerelle wifi Milight
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
port | int | no | Port de la passerelle, 8899 par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of MilightOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of MilightOutputLightRGB
Name | Description
---- | -----------
down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_green 50 | Mettre le canal vert à X pourcent 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 
## More Infos
* LimitlessLED: http://www.limitlessled.com


# MqttInputAnalog
Analog value read from a mqtt broker


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

## Parameters of MqttInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
port | int | no | TCP port of the mqtt broker. Default value is 1883
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of MqttInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

# MqttInputString
String value read from a mqtt broker

## Parameters of MqttInputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | int | no | TCP port of the mqtt broker. Default value is 1883
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.


# MqttInputSwitch
Switch value read from a mqtt broker


Interrupteur de base avec états de appuyé/relâché.

## Parameters of MqttInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
on_value | string | yes | Value to interpret as ON value
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | int | no | TCP port of the mqtt broker. Default value is 1883
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
off_value | string | yes | Value to interpret as OFF value
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.

## Conditions of MqttInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 

# MqttInputTemp
Temperature read from a mqtt broker


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

## Parameters of MqttInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
port | int | no | TCP port of the mqtt broker. Default value is 1883
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of MqttInputTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 

# MqttOutputAnalog
Control analog output through mqtt broker


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

## Parameters of MqttOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with the state (float value) to be sent.
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
port | int | no | TCP port of the mqtt broker. Default value is 1883
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.

## Conditions of MqttOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## Actions of MqttOutputAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 inc 1 | Augmenter valeur par valeur 
 dec | Diminuer la valeur avec le pas définit 
 inc | Augmenter la valeur avec le pas définit 
 0 | Définir une valeur spécifique 
 

# MqttOutputLight
Control lights through mqtt broker


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of MqttOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with with the state (on_value, off_value) to be sent.
on_value | string | yes | Value to interpret as ON value
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
off_value | string | yes | Value to interpret as OFF value
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | int | no | TCP port of the mqtt broker. Default value is 1883
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.

## Conditions of MqttOutputLight
Name | Description
---- | -----------
false | Événement lorsque la lumière est éteinte 
 true | Événement lorsque la lumière est allumée 
 changed | Événement à chaque changement 
 
## Actions of MqttOutputLight
Name | Description
---- | -----------
impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 true | Allumer la lumière 
 

# MqttOutputLightDimmer
Control lights through mqtt broker


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

## Parameters of MqttOutputLightDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with the state (integer value) to be sent.
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | int | no | TCP port of the mqtt broker. Default value is 1883
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.

## Conditions of MqttOutputLightDimmer
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of MqttOutputLightDimmer
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 up 5 | Augmenter l'intensité de X pourcent 
 

# MqttOutputLightRGB
Control RGB lights through mqtt broker


Lumière RVB. Choisir une couleur à définir pour cette lumière.

## Parameters of MqttOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path_brightness | string | yes | The path where to found the brightness value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example 'brightness'
path_y | string | yes | The path where to found the Y (X/Y Color space) value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example color/y, try to read the x value from the color object.
path_x | string | yes | The path where to found the X (X/Y Color space) value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example color/x, try to read the x value from the color object.
data | string | yes | The data sent when publishing color to topic. The __##VALUE_R##__  __##VALUE_G##__  __##VALUE_B##__ or __##VALUE_HEX##__ or __##VALUE_X##__ __##VALUE_Y##__ __##VALUE_BRIGHTNESS##__ contained in data is substituted with the color (integer value or #RRGGBB string value) to be sent.
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | int | no | TCP port of the mqtt broker. Default value is 1883
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.

## Conditions of MqttOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of MqttOutputLightRGB
Name | Description
---- | -----------
down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_green 50 | Mettre le canal vert à X pourcent 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 

# MySensorsInputAnalog
Mesure analogique depuis un noeud MySensors


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

## Parameters of MySensorsInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
node_id | string | yes | ID du noeud sur le réseau MySensors
gateway | list | yes | Passerelle utilisée, tcp ou série
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
host | string | yes | Adresse IP de la passerelle tcp si pertinent
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of MySensorsInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsInputString
Chaine depuis un noeud MySensors

## Parameters of MySensorsInputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
node_id | string | yes | ID du noeud sur le réseau MySensors
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## More Infos
* MySensors: http://mysensors.org


# MySensorsInputSwitch
Interrupteur depuis un noeud MySensors


Interrupteur de base avec états de appuyé/relâché.

## Parameters of MySensorsInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
node_id | string | yes | ID du noeud sur le réseau MySensors
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of MySensorsInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsInputSwitchLongPress
Appui long depuis un noeud MySensors


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

## Parameters of MySensorsInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
node_id | string | yes | ID du noeud sur le réseau MySensors
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of MySensorsInputSwitchLongPress
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 1 | Événement déclenché lors d'un appui court 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsInputSwitchTriple
Triple clic depuis un noeud MySensors


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

## Parameters of MySensorsInputSwitchTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
node_id | string | yes | ID du noeud sur le réseau MySensors
gateway | list | yes | Passerelle utilisée, tcp ou série
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of MySensorsInputSwitchTriple
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un double clic 
 3 | Événement déclenché lors d'un triple clic 
 1 | Événement déclenché lors d'un simple clic 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsInputTemp
Température depuis un noeud MySensors


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

## Parameters of MySensorsInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
node_id | string | yes | ID du noeud sur le réseau MySensors
gateway | list | yes | Passerelle utilisée, tcp ou série
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
host | string | yes | Adresse IP de la passerelle tcp si pertinent
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of MySensorsInputTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsOutputAnalog
Sortie analogique depuis un noeud MySensors


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

## Parameters of MySensorsOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_DIMMER, voir MySensors.cpp pour plus de valeurs.
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
node_id | string | yes | ID du noeud sur le réseau MySensors
host | string | yes | Adresse IP de la passerelle tcp si pertinent
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
gateway | list | yes | Passerelle utilisée, tcp ou série
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.

## Conditions of MySensorsOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## Actions of MySensorsOutputAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 inc 1 | Augmenter valeur par valeur 
 dec | Diminuer la valeur avec le pas définit 
 inc | Augmenter la valeur avec le pas définit 
 0 | Définir une valeur spécifique 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsOutputDimmer
Variateur de lumière avec un noeud MySensors


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

## Parameters of MySensorsOutputDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_DIMMER, voir MySensors.cpp pour plus de valeurs.
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
gateway | list | yes | Passerelle utilisée, tcp ou série
node_id | string | yes | ID du noeud sur le réseau MySensors
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | Adresse IP de la passerelle tcp si pertinent
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud

## Conditions of MySensorsOutputDimmer
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of MySensorsOutputDimmer
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 up 5 | Augmenter l'intensité de X pourcent 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsOutputLight
Lumière/Relai avec un noeud MySensors


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of MySensorsOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_LIGHT, voir MySensors.cpp pour plus de valeurs.
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
gateway | list | yes | Passerelle utilisée, tcp ou série
node_id | string | yes | ID du noeud sur le réseau MySensors
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | Adresse IP de la passerelle tcp si pertinent

## Conditions of MySensorsOutputLight
Name | Description
---- | -----------
false | Événement lorsque la lumière est éteinte 
 true | Événement lorsque la lumière est allumée 
 changed | Événement à chaque changement 
 
## Actions of MySensorsOutputLight
Name | Description
---- | -----------
impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 true | Allumer la lumière 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsOutputLightRGB
Variateur de lumière RVB avec un noeud MySensors


Lumière RVB. Choisir une couleur à définir pour cette lumière.

## Parameters of MySensorsOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_DIMMER, voir MySensors.cpp pour plus de valeurs.
node_id_green | string | yes | ID du noeud pour le canal vert sur le réseau MySensors
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
gateway | list | yes | Passerelle utilisée, tcp ou série
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
sensor_id_green | string | yes | ID du capteur pour le canal vert, tel que défini dans votre noeud
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | Adresse IP de la passerelle tcp si pertinent
sensor_id_blue | string | yes | ID du capteur pour le canal bleu, tel que défini dans votre noeud
node_id_blue | string | yes | ID du noeud pour le canal bleu sur le réseau MySensors
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
node_id_red | string | yes | ID du noeud pour le canal rouge sur le réseau MySensors
sensor_id_red | string | yes | ID du capteur pour le canal rouge, tel que défini dans votre noeud

## Conditions of MySensorsOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of MySensorsOutputLightRGB
Name | Description
---- | -----------
down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_green 50 | Mettre le canal vert à X pourcent 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsOutputShutter
Volet avec un noeud MySensors


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets à impulsions.

## Parameters of MySensorsOutputShutter
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_LIGHT, voir MySensors.cpp pour plus de valeurs.
sensor_id_down | string | yes | ID du capteur pour fermer, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
gateway | list | yes | Passerelle utilisée, tcp ou série
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
node_id_up | string | yes | ID du noeud pour ouvrir sur le réseau MySensors
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
node_id_down | string | yes | ID du noeud pour fermer sur le réseau MySensors
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
sensor_id_up | string | yes | ID du capteur pour ouvrir, tel que défini dans votre noeud
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet
time | int | yes | Temps en sec pour ouvrir ou fermer le volet
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.

## Conditions of MySensorsOutputShutter
Name | Description
---- | -----------
false | Événement lorsque le volet est fermé 
 true | Événement lorsque le volet est ouvert 
 changed | Événement sur toute modification de l'état du volet 
 
## Actions of MySensorsOutputShutter
Name | Description
---- | -----------
impulse up 200 | Ouvrir le volet pendant X ms 
 set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse down 200 | Fermer le volet pendant X ms 
 toggle | Inverser l'état du volet 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 stop | Arrêter le volet 
 down | Fermer le volet 
 up | Ouvrir le volet 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsOutputShutterSmart
Volet intelligent avec un noeud MySensors


Volet intelligent. Cet volet calcule sa position en fonction du temps nécessaire pour s'ouvrir et se fermer. Il est alors possible de régler directement le volet à une position spécifiée.

## Parameters of MySensorsOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_LIGHT, voir MySensors.cpp pour plus de valeurs.
sensor_id_down | string | yes | ID du capteur pour fermer, tel que défini dans votre noeud
host | string | yes | Adresse IP de la passerelle tcp si pertinent
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
gateway | list | yes | Passerelle utilisée, tcp ou série
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
node_id_up | string | yes | ID du noeud pour ouvrir sur le réseau MySensors
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
node_id_down | string | yes | ID du noeud pour fermer sur le réseau MySensors
time_up | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est précis, plus le positionnement sera précis
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
sensor_id_up | string | yes | ID du capteur pour ouvrir, tel que défini dans votre noeud
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
time_down | int | yes | Temps en sec pour que le volet soit complètement fermé. Plus il est précis, plus le positionnement sera précis
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.

## Conditions of MySensorsOutputShutterSmart
Name | Description
---- | -----------
false | Événement lorsque le volet est fermé 
 true | Événement lorsque le volet est ouvert 
 changed | Événement sur toute modification de l'état du volet 
 
## Actions of MySensorsOutputShutterSmart
Name | Description
---- | -----------
up 5 | Ouvrir le volet de X pourcent 
 set 50 | Mettre le volet à la position X en pourcentage 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 down 5 | Fermer le volet de X pourcent 
 impulse down 200 | Fermer le volet pendant X ms 
 toggle | Inverser l'état du volet 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 calibrate | Commencez l'étalonnage du volet. Cela ouvre complètement le volet et réinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 stop | Arrêter le volet 
 down | Fermer le volet 
 up | Ouvrir le volet 
 
## More Infos
* MySensors: http://mysensors.org


# MySensorsOutputString
Chaine envoyé au noeud MySensors

## Parameters of MySensorsOutputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Type de données envoyé au noeud. Par défaut: V_VAR1, voir MySensors.cpp pour plus de valeurs.
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | string | yes | Si vous utilisez la passerelle série, le port est le port série (/dev/ttyUSB0 par ex.). Si vous utilisez la passerelle tcp, c'est le port TCP de la passerelle.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
gateway | list | yes | Passerelle utilisée, tcp ou série
node_id | string | yes | ID du noeud sur le réseau MySensors
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | Adresse IP de la passerelle tcp si pertinent
sensor_id | string | yes | ID du capteur, tel que défini dans votre noeud

## More Infos
* MySensors: http://mysensors.org


# OLAOutputLightDimmer
Variateur de lumière DMX avec OLA (Open Lighting Architecture)


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

## Parameters of OLAOutputLightDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
channel | int | yes | Canal DMX à contrôler
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
universe | int | yes | Univers OLA à contrôler
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of OLAOutputLightDimmer
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of OLAOutputLightDimmer
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 up 5 | Augmenter l'intensité de X pourcent 
 
## More Infos
* OLA: http://www.openlighting.org


# OLAOutputLightRGB
Le variateur de lumière RVB utilise 3 canaux DMX avec OLA (Open Lighting Architecture)


Lumière RVB. Choisir une couleur à définir pour cette lumière.

## Parameters of OLAOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
channel_blue | int | yes | Canal DMX bleu à contrôler
channel_red | int | yes | Canal DMX rouge à contrôler
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
channel_green | int | yes | Canal DMX vert à contrôler
universe | int | yes | Univers OLA à contrôler
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of OLAOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of OLAOutputLightRGB
Name | Description
---- | -----------
down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_green 50 | Mettre le canal vert à X pourcent 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 
## More Infos
* OLA: http://www.openlighting.org


# OutputFake
Fausse sortie de test. Ne fait rien. Ne pas utiliser.

## Parameters of OutputFake
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server


# OWTemp
Capteur de tempérarure OneWire DS18B20


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

## Parameters of OWTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
use_w1 | bool | no | Forcer l'utilisation du pilote kernel w1 au lieu du pilote OneWire
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
ow_id | string | yes | Identifiant du capteur sur le bus OneWire
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
ow_args | string | yes | Paramètre supplémentaire utilisé pour initialiser owfs. Par exemple, -u permet d'utiliser les drivers USB de owfs
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of OWTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/OneWire


# PingInputSwitch
Entrée basée sur le résultat d'une commande ping. Utile pour détecter la présence d'un hôte sur le réseau.


Interrupteur de base avec états de appuyé/relâché.

## Parameters of PingInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
interval | int | no | Intervalle entre 2 pings en ms. 15 sec par défaut 
timeout | int | no | Délai d'attente en ms pour la réponse du ping
host | string | yes | Adresse IP ou nom de l'hôte pour le ping
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of PingInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | L'hôte est n'en pas en ligne et/ou ne répond pas au ping 
 true | L'hôte est en ligne et répond au ping 
 
# Planet - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Encodeur/Camera IP Planet. La caméra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of Planet
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
password | string | no | Mot de passe pour l'utilisateur
username | string | no | Nom d'utilisateur pour accéder à la caméra
model | string | yes | Modèle de caméra (ICA-210, ICA-210W, ICA-300, ICA-302, ICA-500) à utiliser
rotate | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
width | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server


# Roon
Roon audio player allows control of a Roon player from Calaos

## Parameters of Roon
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | no | Static Roon server IP address, empty to autodetect on network
zone_id | string | yes | Roon zone ID
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | yes | Les lecteurs audio ne sont pas affichés dans les pièces
port | int | yes | Static Roon server port, empty to autodetect on network
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of Roon
Name | Description
---- | -----------
onplaylistchange | Événement lors d'une modification de la liste de lecture 
 onvolumechange | Événement lors d'un changement de volume 
 onstop | Événement lors de l'arrêt 
 onsongchange | Événement lors de la lecture d'une nouvelle chanson 
 onpause | Événement lors de la pause 
 onplay | Événement lors de la lecture 
 
## Actions of Roon
Name | Description
---- | -----------
volume down 1 | Diminuer le volume 
 volume up 1 | Augmenter le volume 
 previous | Lire la précédente chanson de la liste de lecture 
 stop | Arrêter la lecture 
 pause | Suspendre la lecture 
 volume set 50 | Régler le volume 
 next | Lire la prochaine chanson de la liste de lecture 
 play | Démarrer la lecture 
 

# Scenario
Une variable de scénario. Utilisez-la comme un bouton virtuel pour lancer un scénario (liste d'actions)

## Parameters of Scenario
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
auto_scenario | string | no | Utilisation interne uniquement pour les scénarios auto. Lecture seule.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of Scenario
Name | Description
---- | -----------
true | Événement déclenché lors du démarrage du scénario 
 
## Actions of Scenario
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Arrêtez le scénario (uniquement pour les scénarios de boucles spéciales) 
 true | Démarrer le scénario 
 

# Squeezebox
#### Alias: slim
Le lecteur audio Squeezebox permet de contrôler une Squeezebox depuis Calaos

## Parameters of Squeezebox
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
port_cli | int | no | Port du serveur, 9090 par défaut
host | string | yes | Adresse IP du serveur multimédia Logitech
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
port_web | int | no | Port de l'interface Web, 9000 par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | yes | Les lecteurs audio ne sont pas affichés dans les pièces
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | ID unique de la squeezebox sur le serveur

## Conditions of Squeezebox
Name | Description
---- | -----------
onplaylistchange | Événement lors d'une modification de la liste de lecture 
 onvolumechange | Événement lors d'un changement de volume 
 onstop | Événement lors de l'arrêt 
 onsongchange | Événement lors de la lecture d'une nouvelle chanson 
 onpause | Événement lors de la pause 
 onplay | Événement lors de la lecture 
 
## Actions of Squeezebox
Name | Description
---- | -----------
add <argument> | Ajouter des pistes à la liste de lecture. <argument> peut être n'importe quel album_id: XX artist_id: XX playlist_id: XX, ... 
 play <argument> | Effacer la liste de lecture et lire argument. <argument> peut être n'importe quel album_id: XX artist_id: XX playlist_id: XX, ... 
 pause | Suspendre la lecture 
 sync <playerid> | Synchronisez ce lecteur avec un autre 
 stop | Arrêter la lecture 
 volume up 1 | Augmenter le volume 
 volume set 50 | Régler le volume 
 previous | Lire la précédente chanson de la liste de lecture 
 volume down 1 | Diminuer le volume 
 play | Démarrer la lecture 
 next | Lire la prochaine chanson de la liste de lecture 
 power on | Allumer le lecteur 
 sleep 10 | Mettre en veille dans X secondes 
 power off | Eteindre le lecteur 
 unsync <playerid> | Arrêtez la synchronisation de ce lecteur 
 
# standard_mjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Camera IP MJPEG/Jpeg. La caméra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of standard_mjpeg
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
ptz | bool | no | Activer si la caméra supporte le PTZ
rotate | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
width | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
url_mjpeg | string | no | URL pour le support du flux Mjpeg
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
url_jpeg | string | yes | URL pour un instantané au format Jpeg
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

# StandardMjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Camera IP MJPEG/Jpeg. La caméra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of StandardMjpeg
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
ptz | bool | no | Activer si la caméra supporte le PTZ
rotate | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
width | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
url_mjpeg | string | no | URL pour le support du flux Mjpeg
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
url_jpeg | string | yes | URL pour un instantané au format Jpeg
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

# SynoSurveillanceStation - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Station de surveillance à cameras IP Synology. La caméra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of SynoSurveillanceStation
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
width | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.
camera_profile | int | no | Profil à utiliser pour l'instantané. 0- Haute qualité, 1- Équilibré, 2- Faible bande-passante
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
rotate | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
url | string | yes | Url complet ver le NAS Synology. Ex: https://192.168.0.22:5000
username | string | yes | Nom d'utilisateur qui a accès à la station de surveillance
camera_id | string | yes | Arrêter la minuterie
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
password | string | yes | Mot de passe pour l'utilisateur


# TeleinfoInputAnalog
Mesure analogique extraite des informations Teleinfo.


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

## Parameters of TeleinfoInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
value | list | yes | Toutes ces valeurs sont lues sur l'équipement de Teleinfo en double précision.
port | string | yes | Port sur lequel obtenir les informations Teleinfo habituellement un port série comme / dev / ttyS0 ou / dev / ttyAMA0
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of TeleinfoInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

# TimeRange
#### Alias: InPlageHoraire
Représentez un objet de plage de temps. Une plage de temps est vraie si l'heure actuelle est dans l'une des gammes incluses, false sinon. La gamme de temps prend également en charge les jours de semaine et les mois.

## Parameters of TimeRange
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Une plage de temps ne peut pas être visible. Toujours faux.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of TimeRange
Name | Description
---- | -----------
changed | Événement déclenché à chaque changement de plage 
 false | Événement déclenché lors de la sortie dans la plage 
 true | Événement déclenché lors de l'entrée dans la plage 
 

# WebInputAnalog
Valeur analogique lue dans un document Web


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

## Parameters of WebInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier téléchargé sera lu et les informations seront extraites du chemin. Par exemple weather[0]/description, essayez de lire la description du premier élément du tableau de l'objet weahter.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier téléchargé est renvoyé en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisée à l'aide du séparateur en tant que délimiteurs. La valeur renvoyée est la valeur à pos dans la ligne fractionnée. Si le séparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
url | string | yes | URL où télécharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
file_type | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of WebInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

# WebInputString
Chaine fournis par un document Web

## Parameters of WebInputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
url | string | yes | URL où télécharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
file_type | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
path | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier téléchargé sera lu et les informations seront extraites du chemin. Par exemple weather[0]/description, essayez de lire la description du premier élément du tableau de l'objet weahter.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier téléchargé est renvoyé en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisée à l'aide du séparateur en tant que délimiteurs. La valeur renvoyée est la valeur à pos dans la ligne fractionnée. Si le séparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server


# WebInputTemp
Température lu dans un document Web


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

## Parameters of WebInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier téléchargé sera lu et les informations seront extraites du chemin. Par exemple weather[0]/description, essayez de lire la description du premier élément du tableau de l'objet weahter.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier téléchargé est renvoyé en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisée à l'aide du séparateur en tant que délimiteurs. La valeur renvoyée est la valeur à pos dans la ligne fractionnée. Si le séparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
url | string | yes | URL où télécharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
file_type | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of WebInputTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 

# WebOutputAnalog
Sortie analogique dans une requete web


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

## Parameters of WebOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
file_type | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
name | string | yes | Nom de l'entrée/sortie.
path | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier téléchargé sera lu et les informations seront extraites du chemin. Par exemple weather[0]/description, essayez de lire la description du premier élément du tableau de l'objet weahter.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier téléchargé est renvoyé en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisée à l'aide du séparateur en tant que délimiteurs. La valeur renvoyée est la valeur à pos dans la ligne fractionnée. Si le séparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
url | string | yes | URL où télécharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.

## Conditions of WebOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## Actions of WebOutputAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 inc 1 | Augmenter valeur par valeur 
 dec | Diminuer la valeur avec le pas définit 
 inc | Augmenter la valeur avec le pas définit 
 0 | Définir une valeur spécifique 
 

# WebOutputLight
Booléen à écrire dans un document Web ou une URL


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of WebOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
url | string | yes | URL pour poster le document. Le champ data est envoyé en POST si il n'est pas nul. Lorsque aucune donnée n'est fournie, Calaos substitue __##VALUE##__ de l'URL avec la valeur à envoyer. Par exemple, si l'url est http://example.com/api?value=__##VALUE##__ l'url sera :
http://example.com/api?value=20.3
L'URL est encodée avant d'être envoyée.
Si l'URL commence par / ou file:// les données sont écrites dans un fichier local.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
data_type | string | yes | Entête HTTP Content-Type utilisé lors de l'envoi du document. Cela dépend du site, mais vous pouvez utiliser application/json ou application/xml comme valeurs correctes.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
data | string | yes | Données à publier. Cette valeur peut être nulle, dans ce cas, la valeur est remplacée dans l'URL, sinon la valeur __##VALUE##__ contenue dans les données est remplacée par la valeur à envoyer.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of WebOutputLight
Name | Description
---- | -----------
false | Événement lorsque la lumière est éteinte 
 true | Événement lorsque la lumière est allumée 
 changed | Événement à chaque changement 
 
## Actions of WebOutputLight
Name | Description
---- | -----------
impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 true | Allumer la lumière 
 

# WebOutputLightRGB
Valeur RVB à écrire dans un document Web ou une URL


Lumière RVB. Choisir une couleur à définir pour cette lumière.

## Parameters of WebOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
url | string | yes | URL pour poster le document. Le champ data est envoyé en POST si il n'est pas nul. Lorsque aucune donnée n'est fournie, Calaos substitue __##VALUE##__ de l'URL avec la valeur à envoyer. Par exemple, si l'url est http://example.com/api?value=__##VALUE##__ l'url sera :
http://example.com/api?value=20.3
L'URL est encodée avant d'être envoyée.
Si l'URL commence par / ou file:// les données sont écrites dans un fichier local.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
data_type | string | yes | Entête HTTP Content-Type utilisé lors de l'envoi du document. Cela dépend du site, mais vous pouvez utiliser application/json ou application/xml comme valeurs correctes.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
data | string | yes | Données à publier. Cette valeur peut être nulle, dans ce cas, la valeur est remplacée dans l'URL, sinon la valeur __##VALUE##__ contenue dans les données est remplacée par la valeur à envoyer.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
raw_value | bool | no | Valeur RVB au format #RRVVBB. Certaines web api prennent seulement le format RRVVBB. Si Raw_value est à vrai, le # est supprimé. La valeur par défaut est faux.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of WebOutputLightRGB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of WebOutputLightRGB
Name | Description
---- | -----------
down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_green 50 | Mettre le canal vert à X pourcent 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 

# WebOutputString
Ecriture d'une chaine dans un document web ou une URL.

## Parameters of WebOutputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
url | string | yes | URL pour poster le document. Le champ data est envoyé en POST si il n'est pas nul. Lorsque aucune donnée n'est fournie, Calaos substitue __##VALUE##__ de l'URL avec la valeur à envoyer. Par exemple, si l'url est http://example.com/api?value=__##VALUE##__ l'url sera :
http://example.com/api?value=20.3
L'URL est encodée avant d'être envoyée.
Si l'URL commence par / ou file:// les données sont écrites dans un fichier local.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
file_type | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
data_type | string | yes | Entête HTTP Content-Type utilisé lors de l'envoi du document. Cela dépend du site, mais vous pouvez utiliser application/json ou application/xml comme valeurs correctes.
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
data | string | yes | Données à publier. Cette valeur peut être nulle, dans ce cas, la valeur est remplacée dans l'URL, sinon la valeur __##VALUE##__ contenue dans les données est remplacée par la valeur à envoyer.
path | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier téléchargé sera lu et les informations seront extraites du chemin. Par exemple weather[0]/description, essayez de lire la description du premier élément du tableau de l'objet weahter.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier téléchargé est renvoyé en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisée à l'aide du séparateur en tant que délimiteurs. La valeur renvoyée est la valeur à pos dans la ligne fractionnée. Si le séparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server


# WIAnalog
#### Alias: WagoInputAnalog
Mesure analogique avec un module Wago (comme 0-10V, 4-20mA, ...)


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

## Parameters of WIAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
port | int | no | Port de l'automate Wage, 502 par défaut
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
var | int | yes | Adresse PLC du capteur d'entrée
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
host | string | yes | Adresse IP de l'automate Wago
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of WIAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/entree_analog


# WIDigitalBP
#### Alias: WIDigital, WagoInputSwitch
Interrupteur avec entrée numérique Modules Wago (comme 750-1405, ...)


Interrupteur de base avec états de appuyé/relâché.

## Parameters of WIDigitalBP
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Adresse IP de l'automate Wago
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
port | int | no | Port de l'automate Wage, 502 par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
var | int | yes | Adresse PLC de l'entrée numérique
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of WIDigitalBP
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


# WIDigitalLong
#### Alias: WagoInputSwitchLongPress
Appui long avec les modules Wago d'entrée numérique (comme 750-1405, ...)


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

## Parameters of WIDigitalLong
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Adresse IP de l'automate Wago
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
port | int | no | Port de l'automate Wage, 502 par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
var | int | yes | Adresse PLC de l'entrée numérique
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of WIDigitalLong
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 1 | Événement déclenché lors d'un appui court 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


# WIDigitalTriple
#### Alias: WagoInputSwitchTriple
Triple clic avec les modules Wago d'entrée numérique (comme 750-1405, ...)


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

## Parameters of WIDigitalTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Adresse IP de l'automate Wago
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
port | int | no | Port de l'automate Wage, 502 par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
var | int | yes | Adresse PLC de l'entrée numérique
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of WIDigitalTriple
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 2 | Événement déclenché lors d'un double clic 
 3 | Événement déclenché lors d'un triple clic 
 1 | Événement déclenché lors d'un simple clic 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


# WITemp
#### Alias: WagoInputTemp
Mesure de la température avec module de température Wago (comme 750-460)


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

## Parameters of WITemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
port | int | no | Port de l'automate Wage, 502 par défaut
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
var | int | yes | Adresse PLC du capteur d'entrée
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
host | string | yes | Adresse IP de l'automate Wago
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of WITemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-460


# WOAnalog
#### Alias: WagoOutputAnalog
Sortie analogique avec module Wago (comme 0-10V, 4-20mA, ...)


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

## Parameters of WOAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
port | int | no | Port de l'automate Wage, 502 par défaut
host | string | yes | Adresse IP de l'automate Wago
var | int | yes | Adresse de l'automate de la sortie
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.

## Conditions of WOAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## Actions of WOAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 inc 1 | Augmenter valeur par valeur 
 dec | Diminuer la valeur avec le pas définit 
 inc | Augmenter la valeur avec le pas définit 
 0 | Définir une valeur spécifique 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/sortie_analog


# WODali
#### Alias: WagoOutputDimmer
Lumière utilisant DALI ou DMX. Pour DALI, vous avez besoin d'un module wago 750-641. Pour DMX, un périphérique DMX4ALL-LAN connecté au Wago PLC.


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

## Parameters of WODali
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | int | no | Port de l'automate Wage, 502 par défaut
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
fade_time | int | no | Durée du fondu DALI (entre 1 et 10)
line | int | no | Ligne de bus DALI, habituellement 1
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
address | int | yes | Adresse de l'appareil. Les adresses DALI se situent entre 1 et 64. Pour les DMX, les adresses commencent à 100. Donc, pour le périphérique DMX 5, l'adresse doit être 105.
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | Adresse IP de l'automate Wago
group | int | no | Réglez sur 1 pour une adresse de groupe DALI, sinon mettre 0

## Conditions of WODali
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of WODali
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 up 5 | Augmenter l'intensité de X pourcent 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-641
* Calaos Wiki: http://calaos.fr/wiki/fr/dmx-lan


# WODaliRVB
#### Alias: WagoOutputDimmerRGB
Lumière RVB utilisant DALI ou DMX. Pour fonctionner, vous avez besoin de 3 canaux DALI/DMX. Pour DALI, vous avez besoin d'un module wago 750-641. Pour DMX, un périphérique DMX4ALL-LAN connecté au Wago PLC.


Lumière RVB. Choisir une couleur à définir pour cette lumière.

## Parameters of WODaliRVB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
bfade_time | int | no | Durée du fondu pour le canal bleu DALI (entre 1 et 10)
bgroup | int | no | Réglez sur 1 si l'adresse du canal bleu est une adresse de groupe DALI, sinon mettre 0
ggroup | int | no | Réglez sur 1 si l'adresse du canal vert est une adresse de groupe DALI, sinon mettre 0
gaddress | int | yes | Adresse pour le canal vert. Les adresses DALI se situent entre 1 et 64. Pour les DMX, les adresses commencent à 100. Donc, pour le périphérique DMX 5, l'adresse doit être 105.
gline | int | no | Ligne de bus DALI pour canal vert, habituellement 1
rfade_time | int | no | Durée du fondu pour le canal rouge DALI (entre 1 et 10)
name | string | yes | Nom de l'entrée/sortie.
baddress | int | yes | Adresse pour le canal bleu. Les adresses DALI se situent entre 1 et 64. Pour les DMX, les adresses commencent à 100. Donc, pour le périphérique DMX 5, l'adresse doit être 105.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | int | no | Port de l'automate Wage, 502 par défaut
gfade_time | int | no | Durée du fondu pour le canal vert DALI (entre 1 et 10)
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
bline | int | no | Ligne de bus DALI pour canal bleu, habituellement 1
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
host | string | yes | Adresse IP de l'automate Wago
rline | int | no | Ligne de bus DALI pour canal rouge, habituellement 1
raddress | int | yes | Adresse pour le canal rouge. Les adresses DALI se situent entre 1 et 64. Pour les DMX, les adresses commencent à 100. Donc, pour le périphérique DMX 5, l'adresse doit être 105.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
rgroup | int | no | Réglez sur 1 si l'adresse du canal rouge est une adresse de groupe DALI,  sinon mettre 0

## Conditions of WODaliRVB
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of WODaliRVB
Name | Description
---- | -----------
down_red 5 | Diminuer l'intensité de X pourcent le canal rouge 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_green 5 | Diminuer l'intensité de X pourcent le canal vert 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 down_blue 5 | Diminuer l'intensité de X pourcent le canal bleu 
 set_state #AA1294 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 up_red 5 | Augmenter l'intensité de X pourcent le canal rouge 
 set_red 50 | Mettre le canal rouge à X pourcent 
 set_blue 50 | Mettre le canal bleu à X pourcent 
 up_green 5 | Augmenter l'intensité de X pourcent le canal vert 
 set_green 50 | Mettre le canal vert à X pourcent 
 set #AA1294 | Définir la couleur. La couleur peut être représentée en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 up_blue 5 | Augmenter l'intensité de X pourcent le canal bleu 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-641
* Calaos Wiki: http://calaos.fr/wiki/fr/dmx-lan


# WODigital
#### Alias: WagoOutputLight
Contrôle d'une lumière ou d'un relais à l'aide des modules de sortie numérique wago (comme 750-1504, ...)


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of WODigital
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
knx | bool | no | Mettre à true si la sortie est un périphérique KNX (uniquement pour 750-849 avec le module KNX / TP1)
var | int | yes | Adresse de l'automate de la sortie
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | int | no | Port de l'automate Wage, 502 par défaut
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | Adresse IP de l'automate Wago
wago_841 | bool | yes | Doit être faux pour le PLC 750-842, sinon vrai

## Conditions of WODigital
Name | Description
---- | -----------
false | Événement lorsque la lumière est éteinte 
 true | Événement lorsque la lumière est allumée 
 changed | Événement à chaque changement 
 
## Actions of WODigital
Name | Description
---- | -----------
impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 true | Allumer la lumière 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


# WOLOutputBool
Objet réveil réseau. Envoye un paquet wake-on-lan à un périphérique sur le réseau.

## Parameters of WOLOutputBool
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
interval | int | no | Intervalle entre 2 pings en ms. 15 sec par défaut 
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
address | string | yes | Adresse MAC de l'hôte à réveiller
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Actions of WOLOutputBool
Name | Description
---- | -----------
true | Envoie un ordre de réveil au périphérique configuré 
 

# WOVolet
#### Alias: WagoOutputShutter
Volet utilisant des modules de sortie numérique wago (comme 750-1504, ...)


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets à impulsions.

## Parameters of WOVolet
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
knx | bool | no | Mettre à true si la sortie est un périphérique KNX (uniquement pour 750-849 avec le module KNX / TP1)
wago_841 | bool | yes | Doit être faux pour le PLC 750-842, sinon vrai
var_down | int | yes | Sortie numérique sur l'automate pour fermer le volet
var_up | int | yes | Sortie numérique sur l'automate pour ouvrir le volet
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
port | int | no | Port de l'automate Wage, 502 par défaut
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet
time | int | yes | Temps en sec pour ouvrir ou fermer le volet
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
host | string | yes | Adresse IP de l'automate Wago

## Conditions of WOVolet
Name | Description
---- | -----------
false | Événement lorsque le volet est fermé 
 true | Événement lorsque le volet est ouvert 
 changed | Événement sur toute modification de l'état du volet 
 
## Actions of WOVolet
Name | Description
---- | -----------
impulse up 200 | Ouvrir le volet pendant X ms 
 set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 impulse down 200 | Fermer le volet pendant X ms 
 toggle | Inverser l'état du volet 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 stop | Arrêter le volet 
 down | Fermer le volet 
 up | Ouvrir le volet 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


# WOVoletSmart
#### Alias: WagoOutputShutterSmart
Volet intelligent utilisant des modules de sortie numérique wago (comme 750-1504, ...)


Volet intelligent. Cet volet calcule sa position en fonction du temps nécessaire pour s'ouvrir et se fermer. Il est alors possible de régler directement le volet à une position spécifiée.

## Parameters of WOVoletSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
knx | bool | no | Mettre à true si la sortie est un périphérique KNX (uniquement pour 750-849 avec le module KNX / TP1)
wago_841 | bool | yes | Doit être faux pour le PLC 750-842, sinon vrai
var_down | int | yes | Sortie numérique sur l'automate pour fermer le volet
var_up | int | yes | Sortie numérique sur l'automate pour ouvrir le volet
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
time_up | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est précis, plus le positionnement sera précis
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
time_down | int | yes | Temps en sec pour que le volet soit complètement fermé. Plus il est précis, plus le positionnement sera précis
port | int | no | Port de l'automate Wage, 502 par défaut
stop_both | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrêt du volet
impulse_time | int | no | Temps d'impulsion en ms pour les volets à impulsion. Si 0, le déclencheur d'impulsion est désactivé. 0 par défaut.
host | string | yes | Adresse IP de l'automate Wago

## Conditions of WOVoletSmart
Name | Description
---- | -----------
false | Événement lorsque le volet est fermé 
 true | Événement lorsque le volet est ouvert 
 changed | Événement sur toute modification de l'état du volet 
 
## Actions of WOVoletSmart
Name | Description
---- | -----------
up 5 | Ouvrir le volet de X pourcent 
 set 50 | Mettre le volet à la position X en pourcentage 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set_state true | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 down 5 | Fermer le volet de X pourcent 
 impulse down 200 | Fermer le volet pendant X ms 
 toggle | Inverser l'état du volet 
 set_state false | Met à jour l'état du volet en interne sans démarrer d'action réelle. Ceci est utile lorsque vous avez mis à jour l'état du volet à partir d'une source externe. 
 calibrate | Commencez l'étalonnage du volet. Cela ouvre complètement le volet et réinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 stop | Arrêter le volet 
 down | Fermer le volet 
 up | Ouvrir le volet 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


# X10Output
Variateur de lumière X10.


Lumière avec variateur. La luminosité peut-être modifiée pour cette lumière.

## Parameters of X10Output
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
code | string | yes | House code de la lumière X10
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of X10Output
Name | Description
---- | -----------
value | Événement lorsque la lumière est à cette valeur 
 changed | Événement à chaque changement 
 
## Actions of X10Output
Name | Description
---- | -----------
hold stop | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 true | Allumer la lumière 
 false | Eteindre la lumière 
 set off 50 | Réglez la valeur de la lumière sans allumer. Ce sera l'intensité lumineuse pour le prochaine allumage 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 down 5 | Diminuer l'intensité de X pourcent 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 hold press | Modifie dynamiquement l'intensité lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 set_state 50 | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 set 50 | Réglez l'intensité lumineuse et allumez si la lumière est éteinte 
 up 5 | Augmenter l'intensité de X pourcent 
 

# xPLInputAnalog
Capteur xPL analogique


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

## Parameters of xPLInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
sensor | string | yes | ID du capteur sur le réseau xPL
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of xPLInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

# xPLInputAnalog
Capteur xPL chaine

## Parameters of xPLInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
sensor | string | yes | ID du capteur sur le réseau xPL
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server


# xPLInputSwitch
Entrée xPL


Interrupteur de base avec états de appuyé/relâché.

## Parameters of xPLInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
sensor | string | yes | ID du capteur sur le réseau xPL
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of xPLInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 

# xPLInputTemp
Capteur de température xPL


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

## Parameters of xPLInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
sensor | string | yes | ID du capteur sur le réseau xPL
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of xPLInputTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 

# xPLOutputAnalog
Sortie analogique pilotée par le protocole xPL


Utile pour contrôler les périphériques de sortie analogique connectés à calaos.

## Parameters of xPLOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
actuator | string | yes | ID de l'actionneur sur le réseau xPL
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
step | float | no | Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.

## Conditions of xPLOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## Actions of xPLOutputAnalog
Name | Description
---- | -----------
dec 1 | Diminuer valeur par valeur 
 inc 1 | Augmenter valeur par valeur 
 dec | Diminuer la valeur avec le pas définit 
 inc | Augmenter la valeur avec le pas définit 
 0 | Définir une valeur spécifique 
 

# xPLOutputAnalog
Sortie analogique pilotée par le protocole xPL

## Parameters of xPLOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
actuator | string | yes | ID de l'actionneur sur le réseau xPL
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server


# xPLOutputSwitch
Lumière/relai piloté par le protocole xPL


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of xPLOutputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
actuator | string | yes | ID de l'actionneur sur le réseau xPL
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
source | string | yes | Nom de la source sur le réseau xPL (Format VendorId-DeviceId.Instance)
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of xPLOutputSwitch
Name | Description
---- | -----------
false | Événement lorsque la lumière est éteinte 
 true | Événement lorsque la lumière est allumée 
 changed | Événement à chaque changement 
 
## Actions of xPLOutputSwitch
Name | Description
---- | -----------
impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 true | Allumer la lumière 
 

# ZibaseAnalogIn
Entrée analogique Zibase. Cet objet peut lire une valeur à partir de périphériques tels que les capteurs de consommation d'énergie, les capteurs de luminosité, ...


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

## Parameters of ZibaseAnalogIn
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_sensor | list | yes | Type of sensor
zibase_id | string | yes | ID du périphérie Zibase (ABC)
port | int | no | Port de la Zibase, 17100 par défaut
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
unit | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
host | string | yes | Adresse IP de la Zibase
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of ZibaseAnalogIn
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

# ZibaseDigitalIn
Entrée numérique Zibase. Cet objet agit comme un commutateur


Interrupteur de base avec états de appuyé/relâché.

## Parameters of ZibaseDigitalIn
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_id2 | string | yes | ID du deuxième périphérie Zibase (ABC)
host | string | yes | Adresse IP de la Zibase
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
zibase_sensor | list | yes | Type of sensor
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
zibase_id | string | yes | ID du premier périphérie Zibase (ABC)
visible | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.
port | int | no | Port de la Zibase, 17100 par défaut
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
name | string | yes | Nom de l'entrée/sortie.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server

## Conditions of ZibaseDigitalIn
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relaché 
 true | Événement déclenché lors de l'appui 
 

# ZibaseDigitalOut
Sortie numérique Zibase. Cet objet contrôle les périphériques Zibase


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of ZibaseDigitalOut
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
name | string | yes | Nom de l'entrée/sortie.
io_style | list | yes | Style d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
port | int | no | Port de la Zibase, 17100 par défaut
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
nbburst | int | no | Nombre d'impulsion à envoyer
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | Adresse IP de la Zibase
zibase_id | string | yes | ID du périphérie Zibase (ABC)
protocol | list | yes | Protocol to use

## Conditions of ZibaseDigitalOut
Name | Description
---- | -----------
false | Événement lorsque la lumière est éteinte 
 true | Événement lorsque la lumière est allumée 
 changed | Événement à chaque changement 
 
## Actions of ZibaseDigitalOut
Name | Description
---- | -----------
impulse 200 | Impulsion sur la lumière. Allume pour X ms puis éteint 
 set_state true | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 toggle | Inverser l'état de la lumière 
 set_state false | Mettre à jour l'état de la lumière interne sans une action réelle. Ceci est utile lorsque vous avez mis à jour l'état de la lumière à partir d'une source externe. 
 false | Eteindre la lumière 
 true | Allumer la lumière 
 

# ZibaseTemp
Capteur de température Zibase


Capteur de température. Utiliser pour afficher la température et contrôler les appareils de chauffage avec des règles basées sur la valeur de la température

## Parameters of ZibaseTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_sensor | list | yes | Type of sensor
zibase_id | string | yes | ID du périphérie Zibase (ABC)
port | int | no | Port de la Zibase, 17100 par défaut
precision | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
name | string | yes | Nom de l'entrée/sortie.
log_history | bool | no | Si activé, ecrit une entrée dans l'historique pour cet IO
visible | bool | no | Affichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
display_warning | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
coeff_a | float | no | Utiliser conjointement coeff_b pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
io_type | string | yes | Type d'entrée/sortie (input, output, inout)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_b | float | no | Utiliser conjointement coeff_a pour appliquer l'équation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
host | string | yes | Adresse IP de la Zibase
offset | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
id | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
gui_type | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
period | float | no | Temps d'échantillonnage en microseconde. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, calaos essaie de lire le paramètre d'intervalle en seconde
interval | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s

## Conditions of ZibaseTemp
Name | Description
---- | -----------
changed | Événement sur toute modification de température 
 value | Événement sur une valeur de température en degrés Celsius 
 
