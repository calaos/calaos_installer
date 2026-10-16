
# AVReceiver
Objet AVReceiver pour contrôler l'amplificateur réseau

## Parameters of AVReceiver
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'appareil
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
model |  | string | yes | Modèle AVReceiver. Pris en charge : pioneer, denon, onkyo, marantz, yamahaModèle AVReceiver. Pris en charge : pionnier, denon, onkyo, marantz, yamaha
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port pour la connexion
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
zone |  | int | no | Zone de l'amplificateur (si pris en charge)

## Actions of AVReceiver
Name | Description
---- | -----------
custom XXXXXX | Envoyer une commande personnalisée au récepteur (si vous connaissez le protocole)Envoier une commande personnalisée au récepteur (si vous connaissez le protocole) 
 power off | Éteindre le récepteur 
 power on | Allumer le récepteur 
 source X | Changer l'entrée actuelle 
 volume 50 | Régler le volume 
 
# Axis - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Encodeur/Camera IP Axis. La camèra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of Axis
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
model |  | string | yes | Modèle de camèra/Canal è utiliser
name | Nom | string | yes | Nom de l'entrée/sortie.
pan_framesize |  | string | no | 
ptz |  | bool | no | Activer si la camèra supporte le PTZ
resolution |  | string | no | Rèsolution è utiliser
rotate |  | int | no | Faire pivoter l'image. Définir une valeur en degrés. Exemple : -90 pour une rotation antihoraire, 90 pour une rotation horaire.
tilt_framesize |  | string | no | 
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
width |  | int | no | Largeur de l'image, si ce paramètre est défini, la vidéo sera redimensionnée pour s'adapter à la largeur donnée. Laissez le paramètre vide pour conserver la taille originale.
zoom_step |  | string | no | 

# Foscam - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Foscam IP Camera/Encoder. Camera can be viewed directly inside calaos and used in rules.

## Parameters of Foscam
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
password |  | string | yes | Mot de passe pour l'utilisateur
port |  | string | yes | Numéro de port
ptz |  | bool | no | Activer si la camèra supporte le PTZ
rotate |  | int | no | Faire pivoter l'image. Définir une valeur en degrés. Exemple : -90 pour une rotation antihoraire, 90 pour une rotation horaire.
username |  | string | yes | Nom d'utilisateur pour accéder à la caméra
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
width |  | int | no | Largeur de l'image, si ce paramètre est défini, la vidéo sera redimensionnée pour s'adapter à la largeur donnée. Laissez le paramètre vide pour conserver la taille originale.
zoom_step |  | string | no | 


# GpioInputSwitch
Entrée GPIO


Interrupteur de base avec états de appuyé/relâché.

## Parameters of GpioInputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low |  | bool | no | Mettre à vrai pour inverser le GPIO
debounce |  | float | no | Debounce time in seconds. Values changing faster than this are filtered out
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gpio |  | int | yes | ID du GPIO sur la carte
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches

## Conditions of GpioInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relâché 
 true | Événement déclenché lors de l'appui 
 

# GpioInputSwitchLongPress
Entrée GPIO


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

## Parameters of GpioInputSwitchLongPress
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low |  | bool | no | Mettre à vrai pour inverser le GPIO
debounce |  | float | no | Debounce time in seconds. Values changing faster than this are filtered out
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gpio |  | int | yes | ID du GPIO sur la carte
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
visible |  | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.

## Conditions of GpioInputSwitchLongPress
Name | Description
---- | -----------
1 | Événement déclenché lors d'un appui court 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 changed | Événement lors de chaque changement d'état 
 

# GpioInputSwitchTriple
Entrée GPIO


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

## Parameters of GpioInputSwitchTriple
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low |  | bool | no | Mettre à vrai pour inverser le GPIO
debounce |  | float | no | Debounce time in seconds. Values changing faster than this are filtered out
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gpio |  | int | yes | ID du GPIO sur la carte
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
visible |  | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.

## Conditions of GpioInputSwitchTriple
Name | Description
---- | -----------
1 | Événement déclenché lors d'un simple clic 
 2 | Événement déclenché lors d'un double clic 
 3 | Événement déclenché lors d'un triple clic 
 changed | Événement lors de chaque changement d'état 
 

# GpioOutputShutter
Volet avec 2 GPIOs


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets è impulsions.

## Parameters of GpioOutputShutter
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low_down |  | bool | no | Mettre à vrai pour inverser le GPIO
active_low_up |  | bool | no | Mettre à vrai pour inverser le GPIO
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gpio_down |  | int | yes | ID du GPIO pour fermer
gpio_up |  | int | yes | ID du GPIO pour ouvrir
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
impulse_time |  | int | no | Temps d'impulsion en ms pour les volets è impulsion. Si 0, le dèclencheur d'impulsion est dèsactivé. 0 par défaut.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
stop_both |  | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrèt du volet
time |  | int | yes | Temps en sec pour ouvrir ou fermer le volet
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of GpioOutputShutter
Name | Description
---- | -----------
changed | Événement sur toute modification de l'état du volet 
 false | Événement lorsque le volet est fermè 
 true | Événement lorsque le volet est ouvert 
 
## Actions of GpioOutputShutter
Name | Description
---- | -----------
down | Fermer le volet 
 impulse down 200 | Fermer le volet pendant X ms 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set_state false | Met è jour l'état du volet en interne sans démarrer d'action rèelle. Ceci est utile lorsque vous avez mis è jour l'état du volet è partir d'une source externe. 
 set_state true | Met è jour l'état du volet en interne sans démarrer d'action rèelle. Ceci est utile lorsque vous avez mis è jour l'état du volet è partir d'une source externe. 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 up | Ouvrir le volet 
 

# GpioOutputShutterSmart
Volet avec 2 GPIOs


Volet intelligent. Cet volet calcule sa position en fonction du temps nècessaire pour s'ouvrir et se fermer. Il est alors possible de règler directement le volet à une position spècifièe.

## Parameters of GpioOutputShutterSmart
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low_down |  | bool | no | Mettre à vrai pour inverser le GPIO
active_low_up |  | bool | no | Mettre à vrai pour inverser le GPIO
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gpio_down |  | int | yes | ID du GPIO pour fermer
gpio_up |  | int | yes | ID du GPIO pour ouvrir
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
impulse_time |  | int | no | Temps d'impulsion en ms pour les volets è impulsion. Si 0, le dèclencheur d'impulsion est dèsactivé. 0 par défaut.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
stop_both |  | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrèt du volet
time_down |  | int | yes | Temps en sec pour que le volet soit complètement fermè. Plus il est prècis, plus le positionnement sera prècis
time_up |  | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est prècis, plus le positionnement sera prècis
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of GpioOutputShutterSmart
Name | Description
---- | -----------
changed | Événement sur toute modification de l'état du volet 
 false | Événement lorsque le volet est fermè 
 true | Événement lorsque le volet est ouvert 
 
## Actions of GpioOutputShutterSmart
Name | Description
---- | -----------
calibrate | Commencez l'ètalonnage du volet. Cela ouvre complètement le volet et rèinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 down | Fermer le volet 
 down 5 | Fermer le volet de X pourcent 
 impulse down 200 | Fermer le volet pendant X ms 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set 50 | Mettre le volet è la position X en pourcentage 
 set_state 50 | Update internal shutter position, in percent, without starting real action. This is useful when having updating the shutter state from an external source. 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 up | Ouvrir le volet 
 up 5 | Ouvrir le volet de X pourcent 
 

# GpioOutputSwitch
Lumière avec un GPIO


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of GpioOutputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low |  | bool | no | Mettre à vrai pour inverser le GPIO
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gpio |  | int | yes | ID du GPIO sur la carte
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of GpioOutputSwitch
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Événement lorsque la lumière est èteinte 
 true | Événement lorsque la lumière est allumèe 
 
## Actions of GpioOutputSwitch
Name | Description
---- | -----------
false | Éteindre la lumière 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis èteint 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes è l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 true | Allumer la lumière 
 

# HueOutputLightRGB
Variateur de lumière RVB avec une Philips Hue


Lumière RVB. Choisir une couleur è dèfinir pour cette lumière.

## Parameters of HueOutputLightRGB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
api |  | string | yes | Clé API renvoyée par la passerelle Hue lorsque l'association a été effectuée. Utilisez Hue Wizard dans Calaos_installer pour obtenir cette valeur automatiquement.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de la passerelle Philips Hue
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
id_hue |  | string | yes | ID unique décrivant la lumière Hue. Cette valeur est renvoyée par Hue Wizard.
insecure |  | bool | no | Skip TLS certificate verification when connecting to the Hue bridge. Default to true (the bridge uses a self-signed certificate). Set to false to only allow a verified HTTPS connection.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of HueOutputLightRGB
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement lorsque la lumière est à cette valeur 
 
## Actions of HueOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Diminuer l'intensitè de X pourcent le canal bleu 
 down_green 5 | Diminuer l'intensitè de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensitè de X pourcent le canal rouge 
 false | Éteindre la lumière 
 set #AA1294 | Définir la couleur. La couleur peut être reprèsentèe en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 set_blue 50 | Mettre le canal bleu è X pourcent 
 set_green 50 | Mettre le canal vert è X pourcent 
 set_red 50 | Mettre le canal rouge è X pourcent 
 set_state #AA1294 | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 true | Allumer la lumière 
 up_blue 5 | Augmenter l'intensitè de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensitè de X pourcent le canal vert 
 up_red 5 | Augmenter l'intensitè de X pourcent le canal rouge 
 
## More Infos
* Meet Hue: http://www.meethue.com


# InputTime
Entrée horaire. Un Événement est déclenché lorsque l'heure actuelle est égale à l'heure configurée. Une date spécifique peut également être définie.

## Parameters of InputTime
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
day |  | int | no | Jour pour l'entrée horaire
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
hour |  | int | yes | Heure pour l'entrée horaire
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
min |  | int | yes | Minute pour l'entrée horaire
month |  | int | no | Mois pour l'entrée horaire
name | Nom | string | yes | Nom de l'entrée/sortie.
sec |  | int | yes | Seconde pour l'entrée horaire
visible |  | bool | no | Une entrée horaire ne peut pas être affichée. Toujours faux
year |  | int | no | Année pour l'entrée horaire

## Conditions of InputTime
Name | Description
---- | -----------
changed | Événement lorsque l'heure change 
 false | Événement lorsque l'heure n'est pas atteinte 
 true | Événement lorsque l'heure est atteinte 
 

# InputTimer
Événement lorsque le temps est écoulé

## Parameters of InputTimer
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
autorestart |  | bool | yes | Redémarrage de la minuterie lorsque le temps est expiré
autostart |  | bool | yes | Démarrage automatique de la minuterie lorsque calaos démarre
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
hour |  | int | yes | Heure pour la minuterie
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
min |  | int | yes | Minutes pour la minuterie
msec |  | int | yes | Milisecondes pour la minuterie
name | Nom | string | yes | Nom de l'entrée/sortie.
sec |  | int | yes | Secondes pour la minuterie
visible |  | bool | no | Une minuterie ne peut pas être affichée. Toujours faux

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
 start | Démarrer la minuterie 
 stop | Arrêter la minuterie 
 

# InternalBool
Booléen interne. Cet objet est utile pour effectuer une programmation interne dans les règles, comme le maintien d'états booléens ou l'affichage de valeurs booléennes

## Parameters of InternalBool
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
rw |  | bool | no | Activer le mode d'édition pour cet objet. Il permet aux utilisateurs de modifier la valeur sur les interfaces. Faux par défaut
save |  | bool | no | Enregistrer automatiquement la valeur dans le cache. La valeur sera rechargée lors du démarrage de calaos. Défaut à faux
value |  | string | no | Initial value, used when save is true and the cache holds no value yet.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of InternalBool
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Événement lorsque le booléen est à faux 
 true | Événement lorsque le booléen est à vrai 
 
## Actions of InternalBool
Name | Description
---- | -----------
false | Mettre la valeur à faux 
 impulse 200 | Faites une impulsion sur une valeur booléenne. Mis à vrai pour X ms puis réinitialiser à faux 
 impulse 500 200 500 200 | Impulsion sur la valeur booléenne avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes à l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 toggle | Inverser la valeur 
 true | Mettre la valeur à vrai 
 

# InternalInt
Objet de numérique interne. Cet objet est utile pour la programmation interne dans les règles, comme les compteurs, d'affichage des valeurs.

## Parameters of InternalInt
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
rw |  | bool | no | Activer le mode d'édition pour cet objet. Il permet aux utilisateurs de modifier la valeur sur les interfaces. Faux par défaut
save |  | bool | no | Enregistrer automatiquement la valeur dans le cache. La valeur sera rechargée lors du démarrage de calaos. Défaut à faux
step |  | float | no | Définir un pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
value |  | string | no | Initial value, used when save is true and the cache holds no value yet.
value_max |  | float | no | Highest value accepted when writing. A higher value is lowered to `value_max`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No upper bound if not set. Ignored, with an error in the log, if lower than `value_min`.
value_min |  | float | no | Lowest value accepted when writing. A lower value is raised to `value_min`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No lower bound if not set. Ignored, with an error in the log, if greater than `value_max`.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of InternalInt
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 
## Actions of InternalInt
Name | Description
---- | -----------
0 | Définir une valeur spécifique 
 dec | Diminuer la valeur avec le pas définit 
 dec 1 | Diminuer valeur par valeur 
 inc | Augmenter la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 

# InternalString
Chaîne interne. Cet objet est utile pour effectuer une programmation interne dans les règles ou afficher des valeurs de texte sur les interfaces utilisateur.

## Parameters of InternalString
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
rw |  | bool | no | Activer le mode d'édition pour cet objet. Il permet aux utilisateurs de modifier la valeur sur les interfaces. Faux par défaut
save |  | bool | no | Enregistrer automatiquement la valeur dans le cache. La valeur sera rechargée lors du démarrage de calaos. Défaut à faux
value |  | string | no | Initial value, used when save is true and the cache holds no value yet.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

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
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
display_warning |  | bool | no | Affiche un avertissement si la valeur n'a pas été mise à jour depuis longtemps. Vrai par défaut
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
interval |  | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group |  | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group |  | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
offset |  | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
precision |  | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

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
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group |  | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group |  | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches

## Conditions of KNXInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relâché 
 true | Événement déclenché lors de l'appui 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputSwitchLongPress
Appui long KNX avec eibnetmux


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

## Parameters of KNXInputSwitchLongPress
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group |  | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group |  | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
visible |  | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.

## Conditions of KNXInputSwitchLongPress
Name | Description
---- | -----------
1 | Événement déclenché lors d'un appui court 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 changed | Événement lors de chaque changement d'état 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputSwitchTriple
Triple clic KNX avec eibnetmux


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

## Parameters of KNXInputSwitchTriple
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group |  | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group |  | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
visible |  | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.

## Conditions of KNXInputSwitchTriple
Name | Description
---- | -----------
1 | Événement déclenché lors d'un simple clic 
 2 | Événement déclenché lors d'un double clic 
 3 | Événement déclenché lors d'un triple clic 
 changed | Événement lors de chaque changement d'état 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputTemp
Tempèrature KNX avec eibnetmux


Temperature sensor input. Used for displaying temperature and to control heating devices with rules based on temperature value

## Parameters of KNXInputTemp
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
display_warning |  | bool | no | Affiche un avertissement si la valeur n'a pas été mise à jour depuis longtemps. Vrai par défaut
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
interval |  | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group |  | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group |  | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
offset |  | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
precision |  | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of KNXInputTemp
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputAnalog
Sortie analogique KNX avec eibnetmux


Utile pour contrôler les périphériques de sortie analogique connectès è calaos.

## Parameters of KNXOutputAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group |  | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group |  | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
step |  | float | no | Définir un pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
value_max |  | float | no | Highest value accepted when writing. A higher value is lowered to `value_max`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No upper bound if not set. Ignored, with an error in the log, if lower than `value_min`.
value_min |  | float | no | Lowest value accepted when writing. A lower value is raised to `value_min`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No lower bound if not set. Ignored, with an error in the log, if greater than `value_max`.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of KNXOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## Actions of KNXOutputAnalog
Name | Description
---- | -----------
0 | Définir une valeur spécifique 
 dec | Diminuer la valeur avec le pas définit 
 dec 1 | Diminuer valeur par valeur 
 inc | Augmenter la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputLight
Lumière KNX avec eibnetmux


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of KNXOutputLight
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group |  | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group |  | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of KNXOutputLight
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Événement lorsque la lumière est èteinte 
 true | Événement lorsque la lumière est allumèe 
 
## Actions of KNXOutputLight
Name | Description
---- | -----------
false | Éteindre la lumière 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis èteint 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes è l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 true | Allumer la lumière 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputLightDimmer
Variateur de lumière KNX avec eibnetmux


Lumière avec variateur. La luminositè peut-être modifièe pour cette lumière.

## Parameters of KNXOutputLightDimmer
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group |  | string | yes | Adresse de groupe KNX, Ex: x/y/z
listen_knx_group |  | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of KNXOutputLightDimmer
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement lorsque la lumière est à cette valeur 
 
## Actions of KNXOutputLightDimmer
Name | Description
---- | -----------
down 5 | Diminuer l'intensitè de X pourcent 
 false | Éteindre la lumière 
 hold press | Changer dynamiquement l'intensité lumineuse en maintenant un interrupteur (action d'appui)Modifie dynamiquement l'intensitè lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 hold stop | Changer dynamiquement l'intensité lumineuse en maintenant un interrupteur (action d'arrêt)Modifie dynamiquement l'intensitè lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis èteint 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes è l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set 50 | Règlez l'intensitè lumineuse et allumez si la lumière est èteinte 
 set off 50 | Règlez la valeur de la lumière sans allumer. Ce sera l'intensitè lumineuse pour le prochaine allumage 
 set_state 50 | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 true | Allumer la lumière 
 up 5 | Augmenter l'intensitè de X pourcent 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputLightRGB
Lumière RVB KNX avec eibnetmux


Lumière RVB. Choisir une couleur è dèfinir pour cette lumière.

## Parameters of KNXOutputLightRGB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group_blue |  | string | yes | Adresse du groupe KNX du canal bleu, ex: x/y/z
knx_group_green |  | string | yes | Adresse du groupe KNX du canal vert, ex: x/y/z
knx_group_red |  | string | yes | Adresse du groupe KNX du canal rouge, ex: x/y/z
listen_knx_group_blue |  | string | no | Adresse du groupe bleu pour l'écoute de l'état, Ex: x/y/z
listen_knx_group_green |  | string | no | Adresse du groupe vert pour l'écoute de l'état, Ex: x/y/z
listen_knx_group_red |  | string | no | Adresse du groupe rouge pour l'écoute de l'état, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of KNXOutputLightRGB
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement lorsque la lumière est à cette valeur 
 
## Actions of KNXOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Diminuer l'intensitè de X pourcent le canal bleu 
 down_green 5 | Diminuer l'intensitè de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensitè de X pourcent le canal rouge 
 false | Éteindre la lumière 
 set #AA1294 | Définir la couleur. La couleur peut être reprèsentèe en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 set_blue 50 | Mettre le canal bleu è X pourcent 
 set_green 50 | Mettre le canal vert è X pourcent 
 set_red 50 | Mettre le canal rouge è X pourcent 
 set_state #AA1294 | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 true | Allumer la lumière 
 up_blue 5 | Augmenter l'intensitè de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensitè de X pourcent le canal vert 
 up_red 5 | Augmenter l'intensitè de X pourcent le canal rouge 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputShutter
Volet KNX avec eibnetmux


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets è impulsions.

## Parameters of KNXOutputShutter
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
impulse_time |  | int | no | Temps d'impulsion en ms pour les volets è impulsion. Si 0, le dèclencheur d'impulsion est dèsactivé. 0 par défaut.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group_down |  | string | yes | Adresse du groupe KNX pour fermer, Ex: x/y/z
knx_group_up |  | string | yes | Adresse du groupe KNX pour ouvrir, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
stop_both |  | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrèt du volet
time |  | int | yes | Temps en sec pour ouvrir ou fermer le volet
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of KNXOutputShutter
Name | Description
---- | -----------
changed | Événement sur toute modification de l'état du volet 
 false | Événement lorsque le volet est fermè 
 true | Événement lorsque le volet est ouvert 
 
## Actions of KNXOutputShutter
Name | Description
---- | -----------
down | Fermer le volet 
 impulse down 200 | Fermer le volet pendant X ms 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set_state false | Met è jour l'état du volet en interne sans démarrer d'action rèelle. Ceci est utile lorsque vous avez mis è jour l'état du volet è partir d'une source externe. 
 set_state true | Met è jour l'état du volet en interne sans démarrer d'action rèelle. Ceci est utile lorsque vous avez mis è jour l'état du volet è partir d'une source externe. 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 up | Ouvrir le volet 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputShutterSmart
Volet KNX avec eibnetmux


Volet intelligent. Cet volet calcule sa position en fonction du temps nècessaire pour s'ouvrir et se fermer. Il est alors possible de règler directement le volet à une position spècifièe.

## Parameters of KNXOutputShutterSmart
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (type de données)
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Nom d'hète de knxd, localhost par défaut
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
impulse_time |  | int | no | Temps d'impulsion en ms pour les volets è impulsion. Si 0, le dèclencheur d'impulsion est dèsactivé. 0 par défaut.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx_group |  | string | yes | Adresse de groupe KNX, Ex: x/y/z
knx_group_down |  | string | yes | Adresse du groupe KNX pour fermer, Ex: x/y/z
knx_group_up |  | string | yes | Adresse du groupe KNX pour ouvrir, Ex: x/y/z
listen_knx_group |  | string | no | Adresse de groupe KNX pour l'écoute de l'état, Ex: x/y/z
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
read_at_start |  | bool | yes | Envoyer une demande de lecture au démarrage pour obtenir la valeur actuelle. Faux par défautEnvoier une demande de lecture au démarrage pour obtenir la valeur. Faux par défaut
stop_both |  | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrèt du volet
time_down |  | int | yes | Temps en sec pour que le volet soit complètement fermè. Plus il est prècis, plus le positionnement sera prècis
time_up |  | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est prècis, plus le positionnement sera prècis
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of KNXOutputShutterSmart
Name | Description
---- | -----------
changed | Événement sur toute modification de l'état du volet 
 false | Événement lorsque le volet est fermè 
 true | Événement lorsque le volet est ouvert 
 
## Actions of KNXOutputShutterSmart
Name | Description
---- | -----------
calibrate | Commencez l'ètalonnage du volet. Cela ouvre complètement le volet et rèinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 down | Fermer le volet 
 down 5 | Fermer le volet de X pourcent 
 impulse down 200 | Fermer le volet pendant X ms 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set 50 | Mettre le volet è la position X en pourcentage 
 set_state 50 | Update internal shutter position, in percent, without starting real action. This is useful when having updating the shutter state from an external source. 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 up | Ouvrir le volet 
 up 5 | Ouvrir le volet de X pourcent 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# MqttInputAnalog
Valeur analogique lue depuis un broker MQTT


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

## Parameters of MqttInputAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
display_warning |  | bool | no | Affiche un avertissement si la valeur n'a pas été mise à jour depuis longtemps. Vrai par défaut
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP du broker MQTT auquel se connecter. La valeur par défaut est 127.0.0.1.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
interval |  | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | Délai de keepalive en secondes. Temps entre deux PING MQTT.
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
offset |  | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
password |  | string | no | Mot de passe à utiliser pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
port |  | int | no | Port TCP du broker MQTT. La valeur par défaut est 1883
precision |  | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
topic_pub |  | string | yes | Sujet sur lequel publier.
topic_sub |  | string | yes | Sujet auquel s'abonner.
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | Utilisateur à utiliser pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

# MqttInputString
Valeur chaîne lue depuis un broker MQTT

## Parameters of MqttInputString
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP du broker MQTT auquel se connecter. La valeur par défaut est 127.0.0.1.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | Délai de keepalive en secondes. Temps entre deux PING MQTT.
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
password |  | string | no | Mot de passe à utiliser pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | Port TCP du broker MQTT. La valeur par défaut est 1883
topic_pub |  | string | yes | Sujet sur lequel publier.
topic_sub |  | string | yes | Sujet auquel s'abonner.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | Utilisateur à utiliser pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.


# MqttInputSwitch
Valeur interrupteur lue depuis un broker MQTT


Interrupteur de base avec états de appuyé/relâché.

## Parameters of MqttInputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP du broker MQTT auquel se connecter. La valeur par défaut est 127.0.0.1.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | Délai de keepalive en secondes. Temps entre deux PING MQTT.
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
off_value |  | string | yes | Valeur à interpréter comme valeur OFF
on_value |  | string | yes | Valeur à interpréter comme valeur ON
password |  | string | no | Mot de passe à utiliser pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | Port TCP du broker MQTT. La valeur par défaut est 1883
topic_pub |  | string | yes | Sujet sur lequel publier.
topic_sub |  | string | yes | Sujet auquel s'abonner.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | Utilisateur à utiliser pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relâché 
 true | Événement déclenché lors de l'appui 
 

# MqttInputTemp
Température lue depuis un broker MQTT


Temperature sensor input. Used for displaying temperature and to control heating devices with rules based on temperature value

## Parameters of MqttInputTemp
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
display_warning |  | bool | no | Affiche un avertissement si la valeur n'a pas été mise à jour depuis longtemps. Vrai par défaut
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP du broker MQTT auquel se connecter. La valeur par défaut est 127.0.0.1.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
interval |  | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | Délai de keepalive en secondes. Temps entre deux PING MQTT.
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
offset |  | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
password |  | string | no | Mot de passe à utiliser pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
port |  | int | no | Port TCP du broker MQTT. La valeur par défaut est 1883
precision |  | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
topic_pub |  | string | yes | Sujet sur lequel publier.
topic_sub |  | string | yes | Sujet auquel s'abonner.
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | Utilisateur à utiliser pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttInputTemp
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

# MqttOutputAnalog
Contrôler une sortie analogique via un broker MQTT


Utile pour contrôler les périphériques de sortie analogique connectès è calaos.

## Parameters of MqttOutputAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
data |  | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with the state (float value) to be sent.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP du broker MQTT auquel se connecter. La valeur par défaut est 127.0.0.1.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | Délai de keepalive en secondes. Temps entre deux PING MQTT.
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
password |  | string | no | Mot de passe à utiliser pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | Port TCP du broker MQTT. La valeur par défaut est 1883
step |  | float | no | Définir un pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
topic_pub |  | string | yes | Sujet sur lequel publier.
topic_sub |  | string | yes | Sujet auquel s'abonner.
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | Utilisateur à utiliser pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.
value_max |  | float | no | Highest value accepted when writing. A higher value is lowered to `value_max`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No upper bound if not set. Ignored, with an error in the log, if lower than `value_min`.
value_min |  | float | no | Lowest value accepted when writing. A lower value is raised to `value_min`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No lower bound if not set. Ignored, with an error in the log, if greater than `value_max`.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## Actions of MqttOutputAnalog
Name | Description
---- | -----------
0 | Définir une valeur spécifique 
 dec | Diminuer la valeur avec le pas définit 
 dec 1 | Diminuer valeur par valeur 
 inc | Augmenter la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 

# MqttOutputLight
Contrôler les lumières via un courtier MQTT


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of MqttOutputLight
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
data |  | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with with the state (on_value, off_value) to be sent.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP du broker MQTT auquel se connecter. La valeur par défaut est 127.0.0.1.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | Délai de keepalive en secondes. Temps entre deux PING MQTT.
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
off_value |  | string | yes | Valeur à interpréter comme valeur OFF
on_value |  | string | yes | Valeur à interpréter comme valeur ON
password |  | string | no | Mot de passe à utiliser pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | Port TCP du broker MQTT. La valeur par défaut est 1883
topic_pub |  | string | yes | Sujet sur lequel publier.
topic_sub |  | string | yes | Sujet auquel s'abonner.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | Utilisateur à utiliser pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttOutputLight
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Événement lorsque la lumière est èteinte 
 true | Événement lorsque la lumière est allumèe 
 
## Actions of MqttOutputLight
Name | Description
---- | -----------
false | Éteindre la lumière 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis èteint 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes è l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 true | Allumer la lumière 
 

# MqttOutputLightDimmer
Contrôler les lumières via un courtier MQTT


Lumière avec variateur. La luminositè peut-être modifièe pour cette lumière.

## Parameters of MqttOutputLightDimmer
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
data |  | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with the state (integer value) to be sent.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP du broker MQTT auquel se connecter. La valeur par défaut est 127.0.0.1.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
in_expr |  | string | no | Use a mathematical expression to convert the raw value into a percent between 0 and 100. The variable `x` is replaced with the raw value. For example, if you want to convert a brightness of 0-254 to a percentage, you can use `x / 2.54`.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | Délai de keepalive en secondes. Temps entre deux PING MQTT.
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
out_expr |  | string | no | Use a mathematical expression to convert the value of the percentage into a raw value. The variable `x` is replaced with the percent value. For example, if you want to convert a percent value of 0-100 to a brightness of 0-254, you can use `x * 2.54`.
password |  | string | no | Mot de passe à utiliser pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | Port TCP du broker MQTT. La valeur par défaut est 1883
topic_pub |  | string | yes | Sujet sur lequel publier.
topic_sub |  | string | yes | Sujet auquel s'abonner.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | Utilisateur à utiliser pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttOutputLightDimmer
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement lorsque la lumière est à cette valeur 
 
## Actions of MqttOutputLightDimmer
Name | Description
---- | -----------
down 5 | Diminuer l'intensitè de X pourcent 
 false | Éteindre la lumière 
 hold press | Changer dynamiquement l'intensité lumineuse en maintenant un interrupteur (action d'appui)Modifie dynamiquement l'intensitè lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 hold stop | Changer dynamiquement l'intensité lumineuse en maintenant un interrupteur (action d'arrêt)Modifie dynamiquement l'intensitè lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis èteint 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes è l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set 50 | Règlez l'intensitè lumineuse et allumez si la lumière est èteinte 
 set off 50 | Règlez la valeur de la lumière sans allumer. Ce sera l'intensitè lumineuse pour le prochaine allumage 
 set_state 50 | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 true | Allumer la lumière 
 up 5 | Augmenter l'intensitè de X pourcent 
 

# MqttOutputLightRGB
Contrôler les lumières RVB via un courtier MQTT


Lumière RVB. Choisir une couleur è dèfinir pour cette lumière.

## Parameters of MqttOutputLightRGB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
data |  | string | yes | The data sent when publishing color to topic. The __##VALUE_R##__  __##VALUE_G##__  __##VALUE_B##__ or __##VALUE_HEX##__ or __##VALUE_X##__ __##VALUE_Y##__ __##VALUE_BRIGHTNESS##__ contained in data is substituted with the color (integer value or #RRGGBB string value) to be sent.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP du broker MQTT auquel se connecter. La valeur par défaut est 127.0.0.1.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | Délai de keepalive en secondes. Temps entre deux PING MQTT.
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
password |  | string | no | Mot de passe à utiliser pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
path_brightness |  | string | yes | The path where to found the brightness value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example 'brightness'
path_x |  | string | yes | The path where to found the X (X/Y Color space) value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example color/x, try to read the x value from the color object.
path_y |  | string | yes | The path where to found the Y (X/Y Color space) value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example color/y, try to read the y value from the color object.
port |  | int | no | Port TCP du broker MQTT. La valeur par défaut est 1883
topic_pub |  | string | yes | Sujet sur lequel publier.
topic_sub |  | string | yes | Sujet auquel s'abonner.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | Utilisateur à utiliser pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttOutputLightRGB
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement lorsque la lumière est à cette valeur 
 
## Actions of MqttOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Diminuer l'intensitè de X pourcent le canal bleu 
 down_green 5 | Diminuer l'intensitè de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensitè de X pourcent le canal rouge 
 false | Éteindre la lumière 
 set #AA1294 | Définir la couleur. La couleur peut être reprèsentèe en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 set_blue 50 | Mettre le canal bleu è X pourcent 
 set_green 50 | Mettre le canal vert è X pourcent 
 set_red 50 | Mettre le canal rouge è X pourcent 
 set_state #AA1294 | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 true | Allumer la lumière 
 up_blue 5 | Augmenter l'intensitè de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensitè de X pourcent le canal vert 
 up_red 5 | Augmenter l'intensitè de X pourcent le canal rouge 
 

# MqttOutputShutter
Control shutters through mqtt broker


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets è impulsions.

## Parameters of MqttOutputShutter
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP du broker MQTT auquel se connecter. La valeur par défaut est 127.0.0.1.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
impulse_time |  | int | no | Temps d'impulsion en ms pour les volets è impulsion. Si 0, le dèclencheur d'impulsion est dèsactivé. 0 par défaut.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | Délai de keepalive en secondes. Temps entre deux PING MQTT.
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
password |  | string | no | Mot de passe à utiliser pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
payload_close |  | string | yes | Payload to send when closing
payload_open |  | string | yes | Payload to send when opening
payload_stop |  | string | yes | Payload to send when stopping
port |  | int | no | Port TCP du broker MQTT. La valeur par défaut est 1883
state_close |  | string | no | Value received for closed state (when topic_sub is set)
state_open |  | string | no | Value received for open state (when topic_sub is set)
stop_both |  | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrèt du volet
time |  | int | yes | Temps en sec pour ouvrir ou fermer le volet
topic_pub |  | string | yes | Topic to publish commands (open/close/stop)
topic_sub |  | string | no | Topic to subscribe to get shutter status (optional). If not set, state is managed by Calaos timing logic.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | Utilisateur à utiliser pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttOutputShutter
Name | Description
---- | -----------
changed | Événement sur toute modification de l'état du volet 
 false | Événement lorsque le volet est fermè 
 true | Événement lorsque le volet est ouvert 
 
## Actions of MqttOutputShutter
Name | Description
---- | -----------
down | Fermer le volet 
 impulse down 200 | Fermer le volet pendant X ms 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set_state false | Met è jour l'état du volet en interne sans démarrer d'action rèelle. Ceci est utile lorsque vous avez mis è jour l'état du volet è partir d'une source externe. 
 set_state true | Met è jour l'état du volet en interne sans démarrer d'action rèelle. Ceci est utile lorsque vous avez mis è jour l'état du volet è partir d'une source externe. 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 up | Ouvrir le volet 
 

# MqttOutputString
Control a string output through mqtt broker

## Parameters of MqttOutputString
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
data |  | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with the string to be sent. If data is a JSON document (starts with `{`, `[` or `"`), the string is JSON-escaped, so write the placeholder inside the quotes: `{"mode":"__##VALUE##__"}`.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP du broker MQTT auquel se connecter. La valeur par défaut est 127.0.0.1.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | Délai de keepalive en secondes. Temps entre deux PING MQTT.
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
password |  | string | no | Mot de passe à utiliser pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | Port TCP du broker MQTT. La valeur par défaut est 1883
topic_pub |  | string | yes | Sujet sur lequel publier.
topic_sub |  | string | yes | Sujet auquel s'abonner.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | Utilisateur à utiliser pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Actions of MqttOutputString
Name | Description
---- | -----------
value | Publish a specific string value 
 

# OLAOutputLightDimmer
Variateur de lumière DMX avec OLA (Open Lighting Architecture)


Lumière avec variateur. La luminositè peut-être modifièe pour cette lumière.

## Parameters of OLAOutputLightDimmer
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
channel |  | int | yes | Canal DMX è contrôler
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
universe |  | int | yes | Univers OLA è contrôler
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of OLAOutputLightDimmer
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement lorsque la lumière est à cette valeur 
 
## Actions of OLAOutputLightDimmer
Name | Description
---- | -----------
down 5 | Diminuer l'intensitè de X pourcent 
 false | Éteindre la lumière 
 hold press | Changer dynamiquement l'intensité lumineuse en maintenant un interrupteur (action d'appui)Modifie dynamiquement l'intensitè lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 hold stop | Changer dynamiquement l'intensité lumineuse en maintenant un interrupteur (action d'arrêt)Modifie dynamiquement l'intensitè lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis èteint 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes è l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set 50 | Règlez l'intensitè lumineuse et allumez si la lumière est èteinte 
 set off 50 | Règlez la valeur de la lumière sans allumer. Ce sera l'intensitè lumineuse pour le prochaine allumage 
 set_state 50 | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 true | Allumer la lumière 
 up 5 | Augmenter l'intensitè de X pourcent 
 
## More Infos
* OLA: http://www.openlighting.org


# OLAOutputLightRGB
Le variateur de lumière RVB utilise 3 canaux DMX avec OLA (Open Lighting Architecture)


Lumière RVB. Choisir une couleur è dèfinir pour cette lumière.

## Parameters of OLAOutputLightRGB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
channel_blue |  | int | yes | Canal DMX bleu è contrôler
channel_green |  | int | yes | Canal DMX vert è contrôler
channel_red |  | int | yes | Canal DMX rouge è contrôler
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
universe |  | int | yes | Univers OLA è contrôler
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of OLAOutputLightRGB
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement lorsque la lumière est à cette valeur 
 
## Actions of OLAOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Diminuer l'intensitè de X pourcent le canal bleu 
 down_green 5 | Diminuer l'intensitè de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensitè de X pourcent le canal rouge 
 false | Éteindre la lumière 
 set #AA1294 | Définir la couleur. La couleur peut être reprèsentèe en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 set_blue 50 | Mettre le canal bleu è X pourcent 
 set_green 50 | Mettre le canal vert è X pourcent 
 set_red 50 | Mettre le canal rouge è X pourcent 
 set_state #AA1294 | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 true | Allumer la lumière 
 up_blue 5 | Augmenter l'intensitè de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensitè de X pourcent le canal vert 
 up_red 5 | Augmenter l'intensitè de X pourcent le canal rouge 
 
## More Infos
* OLA: http://www.openlighting.org


# OutputFake
Fausse sortie de test. Ne fait rien. Ne pas utiliser.

## Parameters of OutputFake
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut


# OWTemp
Capteur de tempèrarure OneWire DS18B20


Temperature sensor input. Used for displaying temperature and to control heating devices with rules based on temperature value

## Parameters of OWTemp
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
display_warning |  | bool | no | Affiche un avertissement si la valeur n'a pas été mise à jour depuis longtemps. Vrai par défaut
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
interval |  | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
offset |  | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
ow_args |  | string | yes | Paramètre supplèmentaire utilisé pour initialiser owfs. Par exemple, -u permet d'utiliser les drivers USB de owfs
ow_id |  | string | yes | Identifiant du capteur sur le bus OneWire
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
precision |  | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
use_w1 |  | bool | no | Forcer l'utilisation du pilote kernel w1 au lieu du pilote OneWire
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of OWTemp
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/OneWire


# PingInputSwitch
Entrée basèe sur le rèsultat d'une commande ping. Utile pour dètecter la présence d'un hôte sur le réseau.


Interrupteur de base avec états de appuyé/relâché.

## Parameters of PingInputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP ou nom de l'hète pour le ping
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
interval |  | int | no | Intervalle entre 2 pings en ms. 15 sec par défaut 
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
timeout |  | int | no | Dèlai d'attente en ms pour la rèponse du ping
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches

## Conditions of PingInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | L'hète est n'en pas en ligne et/ou ne rèpond pas au ping 
 true | L'hète est en ligne et rèpond au ping 
 
# Planet - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Encodeur/Camera IP Planet. La camèra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of Planet
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
model |  | string | yes | Modèle de caméra (ICA-210, ICA-210W, ICA-300, ICA-302, ICA-500) à utiliser
name | Nom | string | yes | Nom de l'entrée/sortie.
password |  | string | no | Mot de passe pour l'utilisateur
rotate |  | int | no | Faire pivoter l'image. Définir une valeur en degrés. Exemple : -90 pour une rotation antihoraire, 90 pour une rotation horaire.
username |  | string | no | Nom d'utilisateur pour accéder à la caméra
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
width |  | int | no | Largeur de l'image, si ce paramètre est défini, la vidéo sera redimensionnée pour s'adapter à la largeur donnée. Laissez le paramètre vide pour conserver la taille originale.


# RemoteUI


Remote UI device. Represents a remote user interface device. Some actions are available to control the device from rules.

## Parameters of RemoteUI
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
auth_token |  | string | no | Authentication token
device_manufacturer |  | string | no | Device manufacturer
device_platform |  | string | no | Device platform
device_secret |  | string | no | Device secret for HMAC validation
device_type |  | list | yes | Device model
device_version |  | string | no | Device version
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
grid_h |  | int | yes | Grid horizontal size
grid_w |  | int | yes | Grid vertical size
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
mac_address |  | string | no | Device MAC address
name | Nom | string | yes | Nom de l'entrée/sortie.
provisioning_code |  | string | yes | Provisioning code for first time setup
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Actions of RemoteUI
Name | Description
---- | -----------
set_brightness X | Set screen brightness (0-100) 
 set_page page_id | Navigate to a specific page by id 
 show_notif message | Show a notification on screen 
 

# RemoteUIOutputRelay


Physical relay on a RemoteUI panel device. Controllable as a standard light output.

## Parameters of RemoteUIOutputRelay
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
relay_num |  | int | yes | Relay number on the device (1, 2, ...)
remote_ui_id |  | string | yes | ID of the parent RemoteUI device
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of RemoteUIOutputRelay
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Événement lorsque la lumière est èteinte 
 true | Événement lorsque la lumière est allumèe 
 
## Actions of RemoteUIOutputRelay
Name | Description
---- | -----------
false | Éteindre la lumière 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis èteint 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes è l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 true | Allumer la lumière 
 

# ReolinkInputSwitch
Interrupteur activé par des événements d'une caméra Reolink


Interrupteur de base avec états de appuyé/relâché.

## Parameters of ReolinkInputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
event_type |  | list | yes | Type d'événement à écouter depuis la caméra Reolink
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
hostname |  | string | yes | Adresse IP ou nom d'hôte de la caméra Reolink
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
password |  | string | yes | Mot de passe pour l'authentification avec la caméra Reolink
username |  | string | yes | Nom d'utilisateur pour l'authentification avec la caméra Reolink
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches

## Conditions of ReolinkInputSwitch
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relâché 
 true | Événement déclenché lors de l'appui 
 

# Roon
Le lecteur audio Roon permet de contrôler un lecteur Roon depuis CalaosLe lecteur audio Roon permet de contrôler un lecteur Roon depuis Calaos

## Parameters of Roon
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | no | Adresse IP statique du serveur Roon, vide pour détection automatique sur le réseauAdresse IP statique du serveur Roon, laisser vide pour la détection automatique sur le réseau
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port statique du serveur Roon, vide pour détection automatique sur le réseauPort statique du serveur Roon, laisser vide pour la détection automatique sur le réseau
visible |  | bool | yes | Les lecteurs audio ne sont pas affichés dans les pièces
zone_id |  | string | yes | ID de la zone Roon

## Conditions of Roon
Name | Description
---- | -----------
onpause | Événement lors de la pause 
 onplay | Événement lors de la lecture 
 onplaylistchange | Événement lors d'une modification de la liste de lecture 
 onsongchange | Événement lors de la lecture d'une nouvelle chanson 
 onstop | Événement lors de l'arrêt 
 onvolumechange | Événement lors d'un changement de volume 
 
## Actions of Roon
Name | Description
---- | -----------
next | Lire la prochaine chanson de la liste de lecture 
 pause | Suspendre la lecture 
 play | Démarrer la lecture 
 previous | Lire la précédente chanson de la liste de lecture 
 stop | Arrêter la lecture 
 volume down 1 | Diminuer le volume 
 volume set 50 | Régler le volume 
 volume up 1 | Augmenter le volume 
 

# Scenario
Une variable de scènario. Utilisez-la comme un bouton virtuel pour lancer un scènario (liste d'actions)

## Parameters of Scenario
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
autoscenario_uid |  | string | no | Utilisation interne uniquement pour les scènarios auto. Lecture seule.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of Scenario
Name | Description
---- | -----------
true | Événement déclenché lors du démarrage du scènario 
 
## Actions of Scenario
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Arrêter le scénario (uniquement pour les scénarios en boucle spéciaux)Arrètez le scènario (uniquement pour les scènarios de boucles spèciales) 
 true | Démarrer le scènario 
 

# Squeezebox
#### Alias: slim
Le lecteur audio Squeezebox permet de contrôler une Squeezebox depuis CalaosLe lecteur audio Squeezebox permet de contrôler une Squeezebox depuis Calaos

## Parameters of Squeezebox
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP du serveur multimédia Logitech
id |  | string | yes | ID unique de la squeezebox sur le serveur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port_cli |  | int | no | Port du serveur, 9090 par défaut
port_web |  | int | no | Port de l'interface Web, 9000 par défaut
visible |  | bool | yes | Les lecteurs audio ne sont pas affichés dans les pièces

## Conditions of Squeezebox
Name | Description
---- | -----------
onpause | Événement lors de la pause 
 onplay | Événement lors de la lecture 
 onplaylistchange | Événement lors d'une modification de la liste de lecture 
 onsongchange | Événement lors de la lecture d'une nouvelle chanson 
 onstop | Événement lors de l'arrêt 
 onvolumechange | Événement lors d'un changement de volume 
 
## Actions of Squeezebox
Name | Description
---- | -----------
add <argument> | Ajouter des pistes à la liste de lecture. <argument> peut être n'importe quel album_id: XX artist_id: XX playlist_id: XX, ... 
 next | Lire la prochaine chanson de la liste de lecture 
 pause | Suspendre la lecture 
 play | Démarrer la lecture 
 play <argument> | Effacer la liste de lecture et lire argument. <argument> peut être n'importe quel album_id: XX artist_id: XX playlist_id: XX, ... 
 power off | Éteindre le lecteur 
 power on | Allumer le lecteur 
 previous | Lire la précédente chanson de la liste de lecture 
 sleep 10 | Mettre en veille dans X secondes 
 stop | Arrêter la lecture 
 sync <playerid> | Synchronisez ce lecteur avec un autre 
 unsync <playerid> | Arrêtez la synchronisation de ce lecteur 
 volume down 1 | Diminuer le volume 
 volume set 50 | Régler le volume 
 volume up 1 | Augmenter le volume 
 
# standard_mjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Camera IP MJPEG/Jpeg. La camèra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of standard_mjpeg
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
ptz |  | bool | no | Activer si la camèra supporte le PTZ
rotate |  | int | no | Faire pivoter l'image. Définir une valeur en degrés. Exemple : -90 pour une rotation antihoraire, 90 pour une rotation horaire.
url_jpeg |  | string | yes | URL pour un instantané au format JPEG
url_mjpeg |  | string | no | URL pour le support du flux MJPEG
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
width |  | int | no | Largeur de l'image, si ce paramètre est défini, la vidéo sera redimensionnée pour s'adapter à la largeur donnée. Laissez le paramètre vide pour conserver la taille originale.

# StandardMjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Camera IP MJPEG/Jpeg. La camèra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of StandardMjpeg
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
ptz |  | bool | no | Activer si la camèra supporte le PTZ
rotate |  | int | no | Faire pivoter l'image. Définir une valeur en degrés. Exemple : -90 pour une rotation antihoraire, 90 pour une rotation horaire.
url_jpeg |  | string | yes | URL pour un instantané au format JPEG
url_mjpeg |  | string | no | URL pour le support du flux MJPEG
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
width |  | int | no | Largeur de l'image, si ce paramètre est défini, la vidéo sera redimensionnée pour s'adapter à la largeur donnée. Laissez le paramètre vide pour conserver la taille originale.

# SynoSurveillanceStation - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Station de surveillance à caméras IP Synology. La caméra peut être directement vue dans calaos et utilisée dans les règles.

## Parameters of SynoSurveillanceStation
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
camera_id |  | string | yes | Arrêter la minuterie
camera_profile |  | int | no | Profil è utiliser pour l'instantanè. 0- Haute qualitè, 1- èquilibrè, 2- Faible bande-passante
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
password |  | string | yes | Mot de passe pour l'utilisateur
rotate |  | int | no | Faire pivoter l'image. Définir une valeur en degrés. Exemple : -90 pour une rotation antihoraire, 90 pour une rotation horaire.
url |  | string | yes | Url complet ver le NAS Synology. Ex: https://192.168.0.22:5000
username |  | string | yes | Nom d'utilisateur qui a accès è la station de surveillance
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
width |  | int | no | Largeur de l'image, si ce paramètre est défini, la vidéo sera redimensionnée pour s'adapter à la largeur donnée. Laissez le paramètre vide pour conserver la taille originale.


# TimeRange
#### Alias: InPlageHoraire
Represent a time range object. A time range is true if current time is in one of the included range, false otherwise. The time range also support weekdays and months. A range whose end is before its start wraps over midnight: it starts on the weekday it is attached to and ends the next morning. For example a range 23:00 -> 01:00 set on monday is true from monday 23:00 to tuesday 01:00, and never on monday between 00:00 and 01:00. Beware that bounds relative to sunrise/sunset move with the season: a range like sunset -> 23:00 is a normal range most of the year but starts to wrap over midnight when sunset gets later than 23:00. Such a range is reported in the log the first time it is evaluated as wrapping.

## Parameters of TimeRange
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
visible |  | bool | no | Une plage de temps ne peut pas être visible. Toujours faux.

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
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
display_warning |  | bool | no | Affiche un avertissement si la valeur n'a pas été mise à jour depuis longtemps. Vrai par défaut
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
file_type |  | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
interval |  | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
offset |  | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
path |  | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier tèlèchargè sera lu et les informations seront extraites du chemin. Les indices de tableau sont un segment de chemin à eux seuls, entre crochets : par exemple weather/[0]/description lit la valeur description du premier élément du tableau weather.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier tèlèchargè est renvoyè en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisèe è l'aide du sèparateur en tant que dèlimiteurs. La valeur renvoyée est la valeur è pos dans la ligne fractionnèe. Si le sèparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
precision |  | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
url |  | string | yes | URL oè tèlècharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of WebInputAnalog
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

# WebInputString
Chaine fournis par un document Web

## Parameters of WebInputString
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
file_type |  | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
path |  | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier tèlèchargè sera lu et les informations seront extraites du chemin. Les indices de tableau sont un segment de chemin à eux seuls, entre crochets : par exemple weather/[0]/description lit la valeur description du premier élément du tableau weather.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier tèlèchargè est renvoyè en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisèe è l'aide du sèparateur en tant que dèlimiteurs. La valeur renvoyée est la valeur è pos dans la ligne fractionnèe. Si le sèparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

url |  | string | yes | URL oè tèlècharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut


# WebInputTemp
Tempèrature lu dans un document Web


Temperature sensor input. Used for displaying temperature and to control heating devices with rules based on temperature value

## Parameters of WebInputTemp
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
display_warning |  | bool | no | Affiche un avertissement si la valeur n'a pas été mise à jour depuis longtemps. Vrai par défaut
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
file_type |  | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
interval |  | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
offset |  | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
path |  | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier tèlèchargè sera lu et les informations seront extraites du chemin. Les indices de tableau sont un segment de chemin à eux seuls, entre crochets : par exemple weather/[0]/description lit la valeur description du premier élément du tableau weather.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier tèlèchargè est renvoyè en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisèe è l'aide du sèparateur en tant que dèlimiteurs. La valeur renvoyée est la valeur è pos dans la ligne fractionnèe. Si le sèparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
precision |  | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
url |  | string | yes | URL oè tèlècharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of WebInputTemp
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 

# WebOutputAnalog
Sortie analogique dans une requete web


Utile pour contrôler les périphériques de sortie analogique connectès è calaos.

## Parameters of WebOutputAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
file_type |  | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
path |  | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier tèlèchargè sera lu et les informations seront extraites du chemin. Les indices de tableau sont un segment de chemin à eux seuls, entre crochets : par exemple weather/[0]/description lit la valeur description du premier élément du tableau weather.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier tèlèchargè est renvoyè en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisèe è l'aide du sèparateur en tant que dèlimiteurs. La valeur renvoyée est la valeur è pos dans la ligne fractionnèe. Si le sèparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

step |  | float | no | Définir un pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
url |  | string | yes | URL oè tèlècharger le document
Si l'URL commence avec / ou file:// les données sont lues sur un disque local
value_max |  | float | no | Highest value accepted when writing. A higher value is lowered to `value_max`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No upper bound if not set. Ignored, with an error in the log, if lower than `value_min`.
value_min |  | float | no | Lowest value accepted when writing. A lower value is raised to `value_min`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No lower bound if not set. Ignored, with an error in the log, if greater than `value_max`.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of WebOutputAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## Actions of WebOutputAnalog
Name | Description
---- | -----------
0 | Définir une valeur spécifique 
 dec | Diminuer la valeur avec le pas définit 
 dec 1 | Diminuer valeur par valeur 
 inc | Augmenter la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 

# WebOutputLight
Booléen è ècrire dans un document Web ou une URL


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of WebOutputLight
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
data |  | string | yes | Donnèes è publier. Cette valeur peut être nulle, dans ce cas, la valeur est remplacèe dans l'URL, sinon la valeur __##VALUE##__ contenue dans les données est remplacèe par la valeur è envoyer.
data_type |  | string | yes | Entète HTTP Content-Type utilisé lors de l'envoi du document. Cela dèpend du site, mais vous pouvez utiliser application/json ou application/xml comme valeurs correctes.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
url |  | string | yes | URL pour poster le document. Le champ data est envoyè en POST si il n'est pas nul. Lorsque aucune donnèe n'est fournie, Calaos substitue __##VALUE##__ de l'URL avec la valeur è envoyer. Par exemple, si l'url est http://example.com/api?value=__##VALUE##__ l'url sera :
http://example.com/api?value=20.3
L'URL est encodèe avant d'être envoyèe.
Si l'URL commence par / ou file:// les données sont ècrites dans un fichier local.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of WebOutputLight
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Événement lorsque la lumière est èteinte 
 true | Événement lorsque la lumière est allumèe 
 
## Actions of WebOutputLight
Name | Description
---- | -----------
false | Éteindre la lumière 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis èteint 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes è l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 true | Allumer la lumière 
 

# WebOutputLightRGB
Valeur RVB è ècrire dans un document Web ou une URL


Lumière RVB. Choisir une couleur è dèfinir pour cette lumière.

## Parameters of WebOutputLightRGB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
data |  | string | yes | Donnèes è publier. Cette valeur peut être nulle, dans ce cas, la valeur est remplacèe dans l'URL, sinon la valeur __##VALUE##__ contenue dans les données est remplacèe par la valeur è envoyer.
data_type |  | string | yes | Entète HTTP Content-Type utilisé lors de l'envoi du document. Cela dèpend du site, mais vous pouvez utiliser application/json ou application/xml comme valeurs correctes.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
raw_value |  | bool | no | Valeur RVB au format #RRVVBB. Certaines web api prennent seulement le format RRVVBB. Si Raw_value est à vrai, le # est supprimè. La valeur par défaut est faux.
url |  | string | yes | URL pour poster le document. Le champ data est envoyè en POST si il n'est pas nul. Lorsque aucune donnèe n'est fournie, Calaos substitue __##VALUE##__ de l'URL avec la valeur è envoyer. Par exemple, si l'url est http://example.com/api?value=__##VALUE##__ l'url sera :
http://example.com/api?value=20.3
L'URL est encodèe avant d'être envoyèe.
Si l'URL commence par / ou file:// les données sont ècrites dans un fichier local.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of WebOutputLightRGB
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement lorsque la lumière est à cette valeur 
 
## Actions of WebOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Diminuer l'intensitè de X pourcent le canal bleu 
 down_green 5 | Diminuer l'intensitè de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensitè de X pourcent le canal rouge 
 false | Éteindre la lumière 
 set #AA1294 | Définir la couleur. La couleur peut être reprèsentèe en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 set_blue 50 | Mettre le canal bleu è X pourcent 
 set_green 50 | Mettre le canal vert è X pourcent 
 set_red 50 | Mettre le canal rouge è X pourcent 
 set_state #AA1294 | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 true | Allumer la lumière 
 up_blue 5 | Augmenter l'intensitè de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensitè de X pourcent le canal vert 
 up_red 5 | Augmenter l'intensitè de X pourcent le canal rouge 
 

# WebOutputString
Ecriture d'une chaine dans un document web ou une URL.

## Parameters of WebOutputString
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
data |  | string | yes | Donnèes è publier. Cette valeur peut être nulle, dans ce cas, la valeur est remplacèe dans l'URL, sinon la valeur __##VALUE##__ contenue dans les données est remplacèe par la valeur è envoyer.
data_type |  | string | yes | Entète HTTP Content-Type utilisé lors de l'envoi du document. Cela dèpend du site, mais vous pouvez utiliser application/json ou application/xml comme valeurs correctes.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
file_type |  | string | yes | Type de fichier. Les valeurs peuvent être xml, json ou text.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
path |  | string | yes | Chemin pour identifier l'emplacement de la valeur. Ce chemin peut prendre plusieurs formes en fonction du type de fichier. Si file_type est JSON, le fichier tèlèchargè sera lu et les informations seront extraites du chemin. Les indices de tableau sont un segment de chemin à eux seuls, entre crochets : par exemple weather/[0]/description lit la valeur description du premier élément du tableau weather.
Si file_type est XML, le chemin d'accès est une expression xpath; Regardez ici la syntaxe: http://www.w3schools.com/xsl/xpath_syntax.asp. Si file_type est TEXT, le fichier tèlèchargè est renvoyè en tant que fichier texte simple, et le chemin doit être sous la forme ligne/pos/separateur. La ligne lue est divisèe è l'aide du sèparateur en tant que dèlimiteurs. La valeur renvoyée est la valeur è pos dans la ligne fractionnèe. Si le sèparateur n'est pas trouvé, la ligne entière est renvoyée. Exemple, pour un fichier contenant 
10,0,10,1,10,2,10,3
20,0,20,1,20,2,20.3
Si le chemin est 2/4/, la valeur retournée sera 20.3

url |  | string | yes | URL pour poster le document. Le champ data est envoyè en POST si il n'est pas nul. Lorsque aucune donnèe n'est fournie, Calaos substitue __##VALUE##__ de l'URL avec la valeur è envoyer. Par exemple, si l'url est http://example.com/api?value=__##VALUE##__ l'url sera :
http://example.com/api?value=20.3
L'URL est encodèe avant d'être envoyèe.
Si l'URL commence par / ou file:// les données sont ècrites dans un fichier local.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut


# WIAnalog
#### Alias: WagoInputAnalog
Mesure analogique avec un module Wago (comme 0-10V, 4-20mA, ...)


Une entrée analogique peut être utilisée pour lire des valeurs analogiques pour les afficher et les utiliser dans les règles.

## Parameters of WIAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
display_warning |  | bool | no | Affiche un avertissement si la valeur n'a pas été mise à jour depuis longtemps. Vrai par défaut
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
interval |  | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
offset |  | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
port |  | int | no | Port de l'automate Wage, 502 par défaut
precision |  | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
var |  | int | yes | Adresse PLC du capteur d'entrée
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

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
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port de l'automate Wage, 502 par défaut
var |  | int | yes | Adresse PLC de l'entrée numérique
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches

## Conditions of WIDigitalBP
Name | Description
---- | -----------
changed | Événement lors de chaque changement d'état 
 false | Événement déclenché lors du relâché 
 true | Événement déclenché lors de l'appui 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


# WIDigitalLong
#### Alias: WagoInputSwitchLongPress
Interrupteur à appui long avec modules d'entrée numérique Wago (comme 750-1405, ...)Appui long avec les modules Wago d'entrée numérique (comme 750-1405, ...)


Interrupteur à appui long. Cet interrupteur prend en charge un appui court et un appui long. L'utilisateur dispose de 500 ms pour effectuer un appui long.

## Parameters of WIDigitalLong
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port de l'automate Wage, 502 par défaut
var |  | int | yes | Adresse PLC de l'entrée numérique
visible |  | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.

## Conditions of WIDigitalLong
Name | Description
---- | -----------
1 | Événement déclenché lors d'un appui court 
 2 | Événement déclenché lors d'un appui long (> 500ms) 
 changed | Événement lors de chaque changement d'état 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


# WIDigitalTriple
#### Alias: WagoInputSwitchTriple
Triple clic avec les modules Wago d'entrée numérique (comme 750-1405, ...)


Commutateur à triple clic. Ce commutateur peut déclencher 3 types d'actions. L'utilisateur dispose de 500 ms pour faire un clic multiple.

## Parameters of WIDigitalTriple
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port de l'automate Wage, 502 par défaut
var |  | int | yes | Adresse PLC de l'entrée numérique
visible |  | bool | no | Un interrupteur ne peut pas être visible. Toujours faux.

## Conditions of WIDigitalTriple
Name | Description
---- | -----------
1 | Événement déclenché lors d'un simple clic 
 2 | Événement déclenché lors d'un double clic 
 3 | Événement déclenché lors d'un triple clic 
 changed | Événement lors de chaque changement d'état 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


# WITemp
#### Alias: WagoInputTemp
Mesure de température avec module de température Wago (comme 750-460)Mesure de la tempèrature avec module de tempèrature Wago (comme 750-460)


Temperature sensor input. Used for displaying temperature and to control heating devices with rules based on temperature value

## Parameters of WITemp
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
display_warning |  | bool | no | Affiche un avertissement si la valeur n'a pas été mise à jour depuis longtemps. Vrai par défaut
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
interval |  | float | no | Temps d'échantillonnage en secondes. La valeur est lue à cette fréquence. Si cette valeur n'est pas définie, la valeur par défaut est 15s
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
offset |  | float | no | Comme le coeff_b, peut être utilisé seul. La valeur par défaut est 0.0
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
port |  | int | no | Port de l'automate Wage, 502 par défaut
precision |  | int | no | Précision de la valeur retournée. La valeur représente le nombre de décimales après le point. La valeur est arrondie comme ceci: valeur = 19.275 => valeur renvoyée 19.28 lors de la précision = 2, 19.3 lorsque la précision = 1, 19 lorsque la précision = 0
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
var |  | int | yes | Adresse PLC du capteur d'entrée
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of WITemp
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-460


# WOAnalog
#### Alias: WagoOutputAnalog
Sortie analogique avec module Wago (comme 0-10V, 4-20mA, ...)


Utile pour contrôler les périphériques de sortie analogique connectès è calaos.

## Parameters of WOAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Utiliser une expression mathématique pour calculer la valeur à partir de la valeur brute. La variable `x` est remplacée par la valeur brute. Par exemple, si vous voulez convertir une valeur brute de 0-1000 en température en Celsius, vous pouvez utiliser `x / 10.0 - 50.0`. Si cette expression est définie, les paramètres coeff_a, coeff_b et offset sont ignorés.
coeff_a |  | float | no | Utiliser conjointement coeff_b pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 1.0.
coeff_b |  | float | no | Utiliser conjointement coeff_a pour appliquer l'èquation de la forme valeur = coeff_a * raw_value + coeff_b. La valeur par défaut est 0.0.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port de l'automate Wage, 502 par défaut
step |  | float | no | Définir un pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0Définit le pas pour incrémenter/décrémenter la valeur. La valeur par défaut est 1.0
unit |  | string | no | Unité affichée sur l'interface utilisateur en tant que suffixe.
value_max |  | float | no | Highest value accepted when writing. A higher value is lowered to `value_max`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No upper bound if not set. Ignored, with an error in the log, if lower than `value_min`.
value_min |  | float | no | Lowest value accepted when writing. A lower value is raised to `value_min`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No lower bound if not set. Ignored, with an error in the log, if greater than `value_max`.
var |  | int | yes | Adresse de l'automate de la sortie
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of WOAnalog
Name | Description
---- | -----------
0 | Événement sur une valeur spécifique 
 changed | Événement à chaque changement 
 value | Événement sur une valeur spécifique 
 
## Actions of WOAnalog
Name | Description
---- | -----------
0 | Définir une valeur spécifique 
 dec | Diminuer la valeur avec le pas définit 
 dec 1 | Diminuer valeur par valeur 
 inc | Augmenter la valeur avec le pas définit 
 inc 1 | Augmenter valeur par valeur 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/sortie_analog


# WODali
#### Alias: WagoOutputDimmer
Lumière utilisant DALI ou DMX. Pour DALI, vous avez besoin d'un module wago 750-641. Pour DMX, un périphérique DMX4ALL-LAN connectè au Wago PLC.


Lumière avec variateur. La luminositè peut-être modifièe pour cette lumière.

## Parameters of WODali
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
address |  | int | yes | Device address. For DALI address is between 1-64. For DMX the address is 100 + the channel, so DMX channel 5 is address 105; channel 1 is address 101 and 100 itself is not a DMX address
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
fade_time |  | int | no | Durèe du fondu DALI (entre 1 et 10)
group |  | int | no | Règlez sur 1 pour une adresse de groupe DALI, sinon mettre 0
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
line |  | int | no | Ligne de bus DALI, habituellement 1
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port de l'automate Wage, 502 par défaut
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of WODali
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement lorsque la lumière est à cette valeur 
 
## Actions of WODali
Name | Description
---- | -----------
down 5 | Diminuer l'intensitè de X pourcent 
 false | Éteindre la lumière 
 hold press | Changer dynamiquement l'intensité lumineuse en maintenant un interrupteur (action d'appui)Modifie dynamiquement l'intensitè lumineuse lorsque vous maintenez un interrupteur (action appuyer) 
 hold stop | Changer dynamiquement l'intensité lumineuse en maintenant un interrupteur (action d'arrêt)Modifie dynamiquement l'intensitè lumineuse lorsque vous maintenez un interrupteur (action relacher) 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis èteint 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes è l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set 50 | Règlez l'intensitè lumineuse et allumez si la lumière est èteinte 
 set off 50 | Règlez la valeur de la lumière sans allumer. Ce sera l'intensitè lumineuse pour le prochaine allumage 
 set_state 50 | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 true | Allumer la lumière 
 up 5 | Augmenter l'intensitè de X pourcent 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-641
* Calaos Wiki: http://calaos.fr/wiki/fr/dmx-lan


# WODaliRVB
#### Alias: WagoOutputDimmerRGB
Lumière RVB utilisant DALI ou DMX. Pour fonctionner, vous avez besoin de 3 canaux DALI/DMX. Pour DALI, vous avez besoin d'un module wago 750-641. Pour DMX, un périphérique DMX4ALL-LAN connectè au Wago PLC.


Lumière RVB. Choisir une couleur è dèfinir pour cette lumière.

## Parameters of WODaliRVB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
baddress |  | int | yes | Device address for blue channel. For DALI address is between 1-64. For DMX the address is 100 + the channel, so DMX channel 5 is address 105; channel 1 is address 101 and 100 itself is not a DMX address
bfade_time |  | int | no | Durèe du fondu pour le canal bleu DALI (entre 1 et 10)
bgroup |  | int | no | Règlez sur 1 si l'adresse du canal bleu est une adresse de groupe DALI, sinon mettre 0
bline |  | int | no | Ligne de bus DALI pour canal bleu, habituellement 1
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gaddress |  | int | yes | Device address for green channel. For DALI address is between 1-64. For DMX the address is 100 + the channel, so DMX channel 5 is address 105; channel 1 is address 101 and 100 itself is not a DMX address
gfade_time |  | int | no | Durèe du fondu pour le canal vert DALI (entre 1 et 10)
ggroup |  | int | no | Règlez sur 1 si l'adresse du canal vert est une adresse de groupe DALI, sinon mettre 0
gline |  | int | no | Ligne de bus DALI pour canal vert, habituellement 1
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port de l'automate Wage, 502 par défaut
raddress |  | int | yes | Device address for red channel. For DALI address is between 1-64. For DMX the address is 100 + the channel, so DMX channel 5 is address 105; channel 1 is address 101 and 100 itself is not a DMX address
rfade_time |  | int | no | Durèe du fondu pour le canal rouge DALI (entre 1 et 10)
rgroup |  | int | no | Règlez sur 1 si l'adresse du canal rouge est une adresse de groupe DALI,  sinon mettre 0
rline |  | int | no | Ligne de bus DALI pour canal rouge, habituellement 1
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Conditions of WODaliRVB
Name | Description
---- | -----------
changed | Événement à chaque changement 
 value | Événement lorsque la lumière est à cette valeur 
 
## Actions of WODaliRVB
Name | Description
---- | -----------
down_blue 5 | Diminuer l'intensitè de X pourcent le canal bleu 
 down_green 5 | Diminuer l'intensitè de X pourcent le canal vert 
 down_red 5 | Diminuer l'intensitè de X pourcent le canal rouge 
 false | Éteindre la lumière 
 set #AA1294 | Définir la couleur. La couleur peut être reprèsentèe en utilisant la notation HTML: #AABBCC, rgb (50, 10, 30), hsl (11, 22, 33) 
 set_blue 50 | Mettre le canal bleu è X pourcent 
 set_green 50 | Mettre le canal vert è X pourcent 
 set_red 50 | Mettre le canal rouge è X pourcent 
 set_state #AA1294 | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière (ON/OFF) 
 true | Allumer la lumière 
 up_blue 5 | Augmenter l'intensitè de X pourcent le canal bleu 
 up_green 5 | Augmenter l'intensitè de X pourcent le canal vert 
 up_red 5 | Augmenter l'intensitè de X pourcent le canal rouge 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-641
* Calaos Wiki: http://calaos.fr/wiki/fr/dmx-lan


# WODigital
#### Alias: WagoOutputLight
Contrèle d'une lumière ou d'un relais è l'aide des modules de sortie numérique wago (comme 750-1504, ...)


Lumière simple. Cette lumière ne comporte que 2 états, ON ou OFF. Peut également être utilisé pour contrôler la sortie des relais simples

## Parameters of WODigital
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
io_style |  | list | yes | Affichage de style GUI. Ceci contrôlera l'ìcône affichée sur l'interface utilisateurStyle d'affichage. Contrôle l'icône affichée sur l'interface utilisateur
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx |  | bool | no | Mettre è true si la sortie est un périphérique KNX (uniquement pour 750-849 avec le module KNX / TP1)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port de l'automate Wage, 502 par défaut
var |  | int | yes | Adresse de l'automate de la sortie
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wago_841 |  | bool | yes | Doit être faux pour le PLC 750-842, sinon vrai

## Conditions of WODigital
Name | Description
---- | -----------
changed | Événement à chaque changement 
 false | Événement lorsque la lumière est èteinte 
 true | Événement lorsque la lumière est allumèe 
 
## Actions of WODigital
Name | Description
---- | -----------
false | Éteindre la lumière 
 impulse 200 | Impulsion sur la lumière. Allume pour X ms puis èteint 
 impulse 500 200 500 200 | Allume la lumière avec un motif.<br>Ex: 500 200 500 200 signifie: VRAI pour 500ms, FAUX pour 200ms, VRAI pour 500ms, FAUX pour 200ms<br>Ex: 500 loop 200 300 signifie: VRAI pour 500ms, puis relancer les prochaines étapes è l'infini, FAUX pour 200ms, VRAI pour 300ms<br>Ex: 100 100 200 old signifie: clignote et reprend l'ancien état (avant le clignotement) 
 set_state false | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 set_state true | Mettre è jour l'état de la lumière interne sans une action rèelle. Ceci est utile lorsque vous avez mis è jour l'état de la lumière è partir d'une source externe. 
 toggle | Inverser l'état de la lumière 
 true | Allumer la lumière 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


# WOLOutputBool
Objet réveil réseau. Envoie un paquet wake-on-lan à un périphérique sur le réseau.

## Parameters of WOLOutputBool
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
address |  | string | yes | Adresse MAC de l'hète è réveiller
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
interval |  | int | no | Intervalle entre 2 pings en ms. 15 sec par défaut 
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Actions of WOLOutputBool
Name | Description
---- | -----------
true | Envoie un ordre de réveil au périphérique configuré 
 

# WOVolet
#### Alias: WagoOutputShutter
Volet simple utilisant les modules de sortie numérique Wago (comme 750-1504, ...)Volet utilisant des modules de sortie numérique wago (comme 750-1504, ...)


Volet simple. Ce volet prend en charge les états ouvert/fermer, ainsi que les volets è impulsions.

## Parameters of WOVolet
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
impulse_time |  | int | no | Temps d'impulsion en ms pour les volets è impulsion. Si 0, le dèclencheur d'impulsion est dèsactivé. 0 par défaut.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx |  | bool | no | Mettre è true si la sortie est un périphérique KNX (uniquement pour 750-849 avec le module KNX / TP1)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port de l'automate Wage, 502 par défaut
stop_both |  | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrèt du volet
time |  | int | yes | Temps en sec pour ouvrir ou fermer le volet
var_down |  | int | yes | Sortie numérique sur l'automate pour fermer le volet
var_up |  | int | yes | Sortie numérique sur l'automate pour ouvrir le volet
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wago_841 |  | bool | yes | Doit être faux pour le PLC 750-842, sinon vrai

## Conditions of WOVolet
Name | Description
---- | -----------
changed | Événement sur toute modification de l'état du volet 
 false | Événement lorsque le volet est fermè 
 true | Événement lorsque le volet est ouvert 
 
## Actions of WOVolet
Name | Description
---- | -----------
down | Fermer le volet 
 impulse down 200 | Fermer le volet pendant X ms 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set_state false | Met è jour l'état du volet en interne sans démarrer d'action rèelle. Ceci est utile lorsque vous avez mis è jour l'état du volet è partir d'une source externe. 
 set_state true | Met è jour l'état du volet en interne sans démarrer d'action rèelle. Ceci est utile lorsque vous avez mis è jour l'état du volet è partir d'une source externe. 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 up | Ouvrir le volet 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


# WOVoletSmart
#### Alias: WagoOutputShutterSmart
Volet intelligent utilisant les modules de sortie numérique Wago (comme 750-1504, ...)Volet intelligent utilisant des modules de sortie numérique wago (comme 750-1504, ...)


Volet intelligent. Cet volet calcule sa position en fonction du temps nècessaire pour s'ouvrir et se fermer. Il est alors possible de règler directement le volet à une position spècifièe.

## Parameters of WOVoletSmart
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
host |  | string | yes | Adresse IP de l'automate Wago
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
impulse_time |  | int | no | Temps d'impulsion en ms pour les volets è impulsion. Si 0, le dèclencheur d'impulsion est dèsactivé. 0 par défaut.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
knx |  | bool | no | Mettre è true si la sortie est un périphérique KNX (uniquement pour 750-849 avec le module KNX / TP1)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
name | Nom | string | yes | Nom de l'entrée/sortie.
port |  | int | no | Port de l'automate Wage, 502 par défaut
stop_both |  | bool | no | En mode impulsion, certains volets doivent activer les deux relais haut et bas lors de l'arrèt du volet
time_down |  | int | yes | Temps en sec pour que le volet soit complètement fermè. Plus il est prècis, plus le positionnement sera prècis
time_up |  | int | yes | Temps en sec pour que le volet soit complètement ouvert. Plus il est prècis, plus le positionnement sera prècis
var_down |  | int | yes | Sortie numérique sur l'automate pour fermer le volet
var_up |  | int | yes | Sortie numérique sur l'automate pour ouvrir le volet
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut
wago_841 |  | bool | yes | Doit être faux pour le PLC 750-842, sinon vrai

## Conditions of WOVoletSmart
Name | Description
---- | -----------
changed | Événement sur toute modification de l'état du volet 
 false | Événement lorsque le volet est fermè 
 true | Événement lorsque le volet est ouvert 
 
## Actions of WOVoletSmart
Name | Description
---- | -----------
calibrate | Commencez l'ètalonnage du volet. Cela ouvre complètement le volet et rèinitialise toutes les valeurs de position internes. Utilisez cette option si la synchronisation du volet est perdue. 
 down | Fermer le volet 
 down 5 | Fermer le volet de X pourcent 
 impulse down 200 | Fermer le volet pendant X ms 
 impulse up 200 | Ouvrir le volet pendant X ms 
 set 50 | Mettre le volet è la position X en pourcentage 
 set_state 50 | Update internal shutter position, in percent, without starting real action. This is useful when having updating the shutter state from an external source. 
 stop | Arrêter le volet 
 toggle | Inverser l'état du volet 
 up | Ouvrir le volet 
 up 5 | Ouvrir le volet de X pourcent 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


# Climatisation pilotée
Un appareil de climatisation qui régule seul (pompe à chaleur, climat ESPHome) : Calaos lit son état et relaie les commandes, il ne régule pas.


Chaque rôle de lien lit l'IO de son lien et écrit son command_io, ou l'IO lui-même quand c'est une sortie. Un rôle qui ne peut pas être écrit n'est pas proposé.

Device of family `climate`, impl `proxy`.

## Parameters of Climatisation pilotée
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
auto_payload (calaos:param) | Valeur pour auto | list | no | Ce qui est écrit sur le lien du mode pour auto. À défaut, la graphie que l'appareil a rapportée en dernier, sinon heat_cool.
changeover_delay (calaos:param) | Délai de bascule chaud/froid | float | no | Avec emulate_range : secondes entre la fin du chauffage et le début du froid, ou l'inverse, au moins min_off_time.
emulate_range (calaos:param) | Double consigne émulée | bool | no | Pour une unité à consigne unique : l'équipement garde lui-même target_temp_low et target_temp_high (IOs internes, créés s'ils manquent) et propose auto. En auto, l'unité passe en chauffage sur la consigne basse sous la plage, en froid sur la consigne haute au-dessus, et s'arrête entre les deux. Demande un lien target_temp, et le chauffage et le froid parmi les modes. La température de la pièce est remote_temp_source, sinon current_temp ; tant qu'elle n'est pas à jour, l'unité reste telle quelle. Un mode changé sur l'unité elle-même suspend auto jusqu'à ce qu'il soit choisi de nouveau. auto n'est jamais envoyé à l'unité.
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
fan_modes (calaos:param) | Modes de ventilation | string | no | Modes de ventilation acceptés par l'appareil, séparés par des virgules. Sans eux, toute commande fan_mode est refusée.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
hysteresis (calaos:param) | Hystérésis | float | no | Avec emulate_range : écart en degrés hors de la plage avant que l'unité démarre, en chauffage sous la consigne basse moins l'hystérésis, en froid au-dessus de la consigne haute plus l'hystérésis. Elle s'arrête dès que la pièce revient dans la plage.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
impl | Implémentation | string | yes | Implémentation de la famille d'équipement : la clé de ce document dans devices.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
min_off_time (calaos:param) | Durée minimale d'arrêt | float | no | Avec emulate_range : durée minimale en secondes d'arrêt de l'unité avant qu'auto la redémarre.
min_on_time (calaos:param) | Durée minimale de marche | float | no | Avec emulate_range : durée minimale en secondes de marche de l'unité en chauffage ou en froid avant qu'auto l'arrête. Un mode choisi par l'utilisateur s'applique tout de suite.
min_range (calaos:param) | Plage minimale | float | no | Écart minimal en degrés entre target_temp_low et target_temp_high. Une plage plus étroite est refusée.
modes (calaos:param) | Modes | string | no | Modes proposés par l'équipement, séparés par des virgules (off, heat, cool, auto, dry, fan_only). Tous par défaut.
name | Nom | string | yes | Nom de l'entrée/sortie.
readback_hold (calaos:param) | Attente de relecture | float | no | Secondes pendant lesquelles une borne de plage qui vient d'être commandée est crue avant que sa relecture la confirme, de sorte que deux commandes partielles rapprochées soient validées ensemble. Un nombre de secondes supérieur à 0, sinon la valeur par défaut est utilisée.
remote_temp_keepalive (calaos:param) | Rafraîchissement de la température distante | float | no | Secondes entre deux relais de la valeur de remote_temp_source vers remote_temp, en plus de chaque changement. L'appareil revient à sa propre sonde quand les mises à jour cessent. Un nombre de secondes supérieur à 0, sinon la valeur par défaut est utilisée.
sensor_timeout (calaos:param) | Délai de la sonde | float | no | Secondes pendant lesquelles une sonde liée peut rester sans relevé, valeur répétée comprise, avant d'être jugée périmée. Un nombre positif ; la valeur par défaut survit à la perte d'un rapport Zigbee horaire. Une sonde dont le dernier relevé a plus de la moitié de ce délai est interrogée quand son protocole le permet (KNX).
swing_modes (calaos:param) | Modes d'oscillation | string | no | Modes d'oscillation acceptés par l'appareil, séparés par des virgules. Sans eux, toute commande swing_mode est refusée.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

## Link roles of Climatisation pilotée
Writable: the device writes this link.

Role | Label | Value | Required | Writable | Description
---- | ----- | ----- | -------- | -------- | -----------
mode | Mode | string | yes | yes | Mode de l'appareil. Demande un command_io sauf si son IO est une sortie. Sa disponibilité est celle de l'appareil : connected à false dans l'état.
current_temp | Température actuelle | number | no | no | Température mesurée par l'appareil.
target_temp | Consigne | number | no | yes | Consigne utilisée en chauffage et en refroidissement.
target_temp_low | Consigne basse | number | no | yes | Borne basse de la plage en auto. Les deux bornes sont nécessaires pour une plage.
target_temp_high | Consigne haute | number | no | yes | Borne haute de la plage en auto. Les deux bornes sont nécessaires pour une plage.
action | Action | string | no | no | Ce que fait l'appareil : off, idle, heating, cooling, drying, fan.
fan_mode | Mode de ventilation | string | no | yes | Mode de ventilation. Demande le paramètre fan_modes.
swing_mode | Mode d'oscillation | string | no | yes | Mode d'oscillation. Demande le paramètre swing_modes.
remote_temp | Température distante | number | no | yes | Où est écrite la température d'une autre sonde, pour les appareils qui en acceptent une. Demande remote_temp_source.
humidity | Sonde d'humidité | number | no | no | Humidité relative remontée par l'appareil, ou par une sonde de la pièce : affichée dans l'état.
remote_temp_source | Sonde de la pièce | number | no | no | La sonde relayée vers remote_temp. Demande remote_temp. Elle n'est pas relayée tant qu'elle n'a pas fourni de relevé depuis le démarrage, ni tant qu'elle n'a rien envoyé pendant sensor_timeout ou se déclare déconnectée : l'appareil revient alors à sa propre sonde.

## Command of Climatisation pilotée
The value is a mode (`off`) or a JSON object holding any of these fields, e.g. `{"mode":"heat","target_temp":21.5}`. A field is only applied when all its links exist and are writable on the instance.

Name | Type | Values or bounds | Requires links | Description
---- | ---- | ---------------- | -------------- | -----------
mode | enum | off, heat, cool, auto, dry, fan_only | mode | Mode de fonctionnement. Les modes acceptés par une instance sont publiés dans ses capacités.
target_temp | number | 7 to 35, step 0.5 °C | target_temp | La consigne utilisée en chauffage et en refroidissement. Les bornes sont celles de l'IO lié (value_min, value_max, step, unit), sinon 7 à 35 par pas de 0,5 en °C. Les bornes effectives sont publiées dans les capacités.
target_temp_low | number | 7 to 35, step 0.5 °C | target_temp_low, target_temp_high | Borne basse de la plage utilisée en auto. Les bornes sont celles de l'IO lié (value_min, value_max, step, unit), sinon 7 à 35 par pas de 0,5 en °C. Les bornes effectives sont publiées dans les capacités.
target_temp_high | number | 7 to 35, step 0.5 °C | target_temp_low, target_temp_high | Borne haute de la plage utilisée en auto. Les bornes sont celles de l'IO lié (value_min, value_max, step, unit), sinon 7 à 35 par pas de 0,5 en °C. Les bornes effectives sont publiées dans les capacités.
fan_mode | enum | values of the fan_modes param | fan_mode | Mode de ventilation, l'un de ceux du paramètre fan_modes.
swing_mode | enum | values of the swing_modes param | swing_mode | Mode d'oscillation, l'un de ceux du paramètre swing_modes.


# Thermostat
Un thermostat qui régule dans Calaos : il commute output_heat et output_cool à partir de current_temp et de ses consignes. Son mode est mémorisé par l'équipement.


Le réduit de nuit, le mode absence et autres sont des règles qui écrivent les consignes ou le mode. Les IOs de consigne et d'action manquants sont créés au chargement.

Device of family `climate`, impl `thermostat`.

## Parameters of Thermostat
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
emitter (calaos:param) | Émetteur | list | no | Ce que les sorties commandent. Seul floor a un effet : avec un lien humidity, le refroidissement par le sol s'arrête alors à humidity_max (point de rosée).
enabled | Activé | bool | no | Activez l'entrée/sortie. Vrai par défaut. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.
gui_type | Type d'affichage | string | no | Type graphique interne pour tous les objets calaos. Paramètre en lecture seule, définit automatiquement
humidity_max (calaos:param) | Humidité maximale | float | no | Humidité relative (%) à partir de laquelle le refroidissement par le sol s'arrête, avec emitter=floor. Pas négative.
hysteresis (calaos:param) | Hystérésis | float | no | Degrés au-delà de la consigne avant qu'une sortie commence à s'enclencher : chauffage sous la consigne moins l'hystérésis, refroidissement au-dessus de la consigne plus l'hystérésis. L'arrêt a lieu dès que la consigne est franchie.
id | Identifiant | string | yes | Identifiant unique de l'entrée/sortie dans calaos-server
impl | Implémentation | string | yes | Implémentation de la famille d'équipement : la clé de ce document dans devices.
io_type | Type d'IO | string | yes | Type d'entrée/sortie (input, output, inout)
log_history | Journal d'historique | bool | no | Si activé, écrit une entrée dans l'historique pour cet IO
logged | InfluxDB | bool | no | Si activé, et si influxdb est activé dans local_config, envoie la valeur à influxdb pour cet IO
min_off_time (calaos:param) | Durée minimale d'arrêt | float | no | Secondes pendant lesquelles une sortie reste arrêtée au minimum avant de pouvoir redémarrer.
min_on_time (calaos:param) | Durée minimale de marche | float | no | Secondes pendant lesquelles une sortie reste enclenchée au minimum, sauf si le mode est off ou si une coupure de sécurité l'arrête.
min_range (calaos:param) | Plage minimale | float | no | Écart minimal en degrés entre target_temp_low et target_temp_high. Une plage plus étroite est refusée.
modes (calaos:param) | Modes | string | no | Modes proposés par le thermostat, séparés par des virgules, parmi heat, cool et auto ; off est toujours proposé. Sans lui, les sorties décident : heat avec output_heat, cool avec output_cool, auto avec les deux, et heat seul avec output. Un mode que les sorties ne peuvent pas assurer, ou inconnu, est ignoré.
name | Nom | string | yes | Nom de l'entrée/sortie.
put_back_delay (calaos:param) | Délai de reprise de la sortie | float | no | Secondes avant de reprendre la main sur une sortie que quelque chose d'autre continue de commuter ; l'attente double tant que l'autre source la défait, jusqu'à put_back_delay_max. Au moins 0,001, sinon la valeur par défaut est utilisée.
put_back_delay_max (calaos:param) | Délai maximal de reprise de la sortie | float | no | Attente la plus longue entre deux reprises de la sortie, en secondes. Pas en dessous de put_back_delay. Elle fixe aussi la fenêtre du journal des écritures extérieures sur une sortie : une ligne par source et par sortie pendant ce temps.
sensor_timeout (calaos:param) | Délai de la sonde | float | no | Secondes pendant lesquelles une sonde liée peut rester sans relevé, valeur répétée comprise, avant d'être jugée périmée. Un nombre positif ; la valeur par défaut survit à la perte d'un rapport Zigbee horaire. Une sonde dont le dernier relevé a plus de la moitié de ce délai est interrogée quand son protocole le permet (KNX).
valid_max (calaos:param) | Température valide maximale | float | no | Plus haute lecture plausible de current_temp, dans l'unité de la sonde. Une lecture au-dessus est traitée comme une absence de lecture, comme valid_min. Pas de contrôle si vide. Pas en dessous de valid_min.
valid_min (calaos:param) | Température valide minimale | float | no | Plus basse lecture plausible de current_temp, dans l'unité de la sonde. Une lecture en dessous est traitée comme une absence de lecture : les sorties passent OFF aussitôt, quel que soit min_on_time, jusqu'à ce qu'une lecture revienne dans les bornes. Pas de contrôle si vide.
visible | Visible | bool | no | Afficher l'entrée/sortie sur toutes les interfaces utilisateur si activé. Vrai par défautAffichez l'entrée/sortie sur toutes les interfaces utilisateur. Vrai par défaut

List `modes`: always offers off. Without it and with output linked: heat. auto needs heat, cool.

## Link roles of Thermostat
Writable: the device writes this link.

Role | Label | Value | Required | Writable | Description
---- | ----- | ----- | -------- | -------- | -----------
current_temp | Température actuelle | number | yes | no | Température lue par la régulation. Les sorties restent arrêtées tant qu'elle n'a pas fourni de relevé depuis le démarrage (une valeur en cache ou retenue ne compte pas), tant qu'elle n'a rien envoyé pendant sensor_timeout, se déclare déconnectée ou lit hors de valid_min..valid_max.
target_temp | Consigne | number | yes | yes | Consigne utilisée en chauffage et en refroidissement. Créée si elle manque.
target_temp_low | Consigne basse | number | no | yes | Borne basse de la plage en auto. Créée si elle manque, avec output_cool.
target_temp_high | Consigne haute | number | no | yes | Borne haute de la plage en auto. Créée si elle manque, avec output_cool.
action | Action | string | no | yes | Écrite par le thermostat : off, idle, heating ou cooling. Créée si elle manque.
output_heat | Sortie chauffage | bool | yes, or output | yes | Sortie commandée pour le chauffage. Active le mode chauffage. Une sortie appartient à un seul thermostat : un second qui la lie ignore le lien.
output_cool | Sortie refroidissement | bool | no | yes | Sortie commandée pour le refroidissement. Active les modes refroidissement et auto. Une sortie appartient à un seul thermostat : un second qui la lie ignore le lien.
output | Vanne de zone | bool | no | yes | Vanne ou relais de la zone, active pour toute demande, chauffage ou refroidissement : le mode donne le sens. À la place d'output_heat et d'output_cool, qui ne peuvent pas être liés avec elle ; le param modes dit ce qu'elle propose. Une sortie appartient à un seul thermostat : un second qui la lie ignore le lien. Replaces output_heat, output_cool.
humidity | Sonde d'humidité | number | no | no | Humidité relative de la pièce. Avec emitter=floor, le refroidissement s'arrête tant qu'elle indique humidity_max ou plus, qu'elle n'a pas de valeur, qu'elle est périmée ou déconnectée. Sans elle, un sol refroidit sans coupure au point de rosée.

## Command of Thermostat
The value is a mode (`off`) or a JSON object holding any of these fields, e.g. `{"mode":"heat","target_temp":21.5}`. A field is only applied when all its links exist and are writable on the instance.

Name | Type | Values or bounds | Requires links | Description
---- | ---- | ---------------- | -------------- | -----------
mode | enum | off, heat (needs output_heat), cool (needs output_cool), auto (needs output_heat + output_cool) |  | Mode de fonctionnement. Les modes acceptés par une instance sont publiés dans ses capacités. Le mode chauffage demande le lien output_heat, le mode refroidissement demande output_cool et le mode auto demande les deux ; avec le lien output, le param modes dit lesquels.
target_temp | number | 7 to 35, step 0.5 °C | target_temp | La consigne utilisée en chauffage et en refroidissement. Les bornes sont celles de l'IO lié (value_min, value_max, step, unit), sinon 7 à 35 par pas de 0,5 en °C. Les bornes effectives sont publiées dans les capacités.
target_temp_low | number | 7 to 35, step 0.5 °C | target_temp_low, target_temp_high | Borne basse de la plage utilisée en auto. Les bornes sont celles de l'IO lié (value_min, value_max, step, unit), sinon 7 à 35 par pas de 0,5 en °C. Les bornes effectives sont publiées dans les capacités.
target_temp_high | number | 7 to 35, step 0.5 °C | target_temp_low, target_temp_high | Borne haute de la plage utilisée en auto. Les bornes sont celles de l'IO lié (value_min, value_max, step, unit), sinon 7 à 35 par pas de 0,5 en °C. Les bornes effectives sont publiées dans les capacités.

