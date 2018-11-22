#!/bin/bash
#
# Name: pam_panic_config
# Description: Create a pam_panic configuration.
# Author: Bandie <bandie@chaospott.de>
#

CONFIGFILE="/etc/pam.d/pampanic"
LHBU="$HOME/LUKSHeaderBackup"

# Set on build time
SECUREDIR="__SECURELIBDIR__"
PAMPANICPW="__PAMPANICPW__"


# Localization
N_(){
  gettext "pam_panic" "$1"
}

NOT_BUILT=$(N_ "ERROR: Bash script was not built correctly.")
RUN_AS_ROOT=$(N_ "Please run this script as root or use sudo.")
BACKTITLE=$(N_ "pam_panic's Configuration Generator.")
BYE=$(N_ "Bye! :)")
OK=$(N_ "OK")
CANCEL=$(N_ "Cancel")
AUTH=$(N_ "Authentication")
PANIC=$(N_ "Panic")
PREWARN_REMOVE=$(N_ "Please remove all removable media devices before you continue.")
PREWARN_GPT=$(N_ "Note, if your device doesn't show up in the upcoming list it might not be a GPT formatted device.")
PREWARN_INSERT_AUTH=$(N_ "Please insert the device you want to use as Authentication device and press OK.")
PREWARN_INSERT_PANIC=$(N_ "Please insert the device you want to use as Panic device and press OK.")
TITLE_RM_AUTH=$(N_ "Removable media: Authentication device")
TITLE_RM_PANIC=$(N_ "Removable media: Panic device")
CHOOSE_DEV=$(N_ "Choose your device:")
DETECT_DEV=$(N_ "Detecting devices...")
WELCOME=$(N_ "Welcome!")
WELCOME1=$(N_ "Welcome to pam_panic's Configuration Generator.")
WELCOME2=$(N_ "It will help you to create a valid pam_panic setup. It will also generate a PAM configuration file.")
WELCOME3=$(N_ "After you're done with this Configuration Generator, you will see some hints how to integrate the new PAM configuration file in your system.")
AUTH_MODE=$(N_ "Authentication mode")
PASSWORDS=$(N_ "Passwords")
REM_MEDIA=$(N_ "Media")
CHOOSE_AUTH1=$(N_ "You can choose between the \"two removable media\" option and the \"two passwords\" option.")
CHOOSE_AUTH2=$(N_ "See \"Help\" to learn what it is.")
CHOOSE_AUTH3=$(N_ "Removable media or passwords?")
UUID_AUTH=$(N_ "Authentication device chosen with UUID")
UUID_PANIC=$(N_ "Panic device chosen with UUID")
ASK_SET_PW=$(N_ "Do you want to set the passwords now?")
SET_PW_FAILED=$(N_ "Failed to set the passwords. :(")
PAM_PANICS_BEHAVIOUR=$(N_ "pam_panic's behaviour")
ASK_SERIOUS1=$(N_ "Do you wish to destroy your LUKS header key slots in case of emergency?")
ASK_SERIOUS2=$(N_ "This means that your encrypted root partition won't be decryptable anymore. After this question we will create a LUKS header backup, if you wish.")
MSG_SERIOUS1=$(N_ "We will destroy")
MSG_SERIOUS2=$(N_ "when you trigger the panic function.")
ASK_LUKS_BU1=$(N_ "Do you want to make a LUKS-Header backup now?")
ASK_LUKS_BU2=$(N_ "Save location:")
LUKS_BU_SAVED=$(N_ "The LUKSHeader backup has been saved here:")
SERIOUS_ERROR=$(N_ "ERROR: There is no encrypted root partition on /dev/sda.")
REBOOT=$(N_ "Reboot")
SHUTDOWN=$(N_ "Shutdown")
NOTHING=$(N_ "Nothing")
ASK_EXTENDED_BEHAVIOUR=$(N_ "Do you wish a reboot or a shutdown after issuing the panic function? Or shall we do nothing at all?") 
TITLE_STRICT=$(N_ "Strict mode")
ASK_STRICT=$(N_ "Do you want to use the strict mode? It means that pam_panic will lock you out and reject any logins if the configuration is corrupt (like a missing password database).")
GEN_CONFIG=$(N_ "Generating configuration...")
CONFIG_EXISTS=$(N_ "Config file exists")
CONFIG_OVERWRITE=$(N_ "exists. Overwrite it?")

WHATNOW=$(N_ "What now?")
SAVEDTO=$(N_ "Now we saved our configuration to")
APPLY_THEM=$(N_ "If you want to let them apply to the other modules, proceed as follows:")
OPEN_MOD=$(N_ "Open a module configuration in")
TRY_OUT=$(N_ "You can try out:")
AND=$(N_ "and")
APPEND=$(N_ "After the line \"#%PAM-1.0\" append")
UBUNTU_NOTICE=$(N_ "On Ubuntu you might want to separate the above mentioned lines on the files common-auth for the \"auth\" part and common-account for the \"account\" part.")
PAMPANIC_ACTIVE=$(N_ "Once you have changed and saved those files, pam_panic will be active.")
NEXT_LOGIN=$(N_ "At your next login you need to")
TYPEPW=$(N_ "type your pam_panic authentification password or")
INSRM=$(N_ "insert your removable authentication media")
PREVTOREG=$(N_ "previous your regular user password.")
QUESTIONS=$(N_ "If you got any question, don't hesitate to ask via IRC (chat.freenode.net in room #pampanic) or via mail + GPG.")
PRESSENTER=$(N_ "Press enter to exit.")


 

[[ ! -d $SECUREDIR ]] || [[ ! -f $PAMPANICPW ]]  && { echo "$NOT_BUILT" ; exit 1 ; }

if [ $EUID -ne 0 ]; then
  echo "$RUN_AS_ROOT"
  exit 1
fi


# Call when using the Cancel button
function cancel(){
  rm -f .pam_panic_media_choice
  clear
  echo "$BYE"
  exit 0
}

# Call when CTRL+C
trap "cancel" INT


# Check, if $1 is a gpt formatted device
function checkGPT(){
  blkid $1 -t PTTYPE=gpt >> /dev/null
  return $?
}


# Get the GPT PartitionUUID
function getPARTUUID(){
  blkid $1 | awk '{print $4;}' | sed 's/PARTUUID="//;s/"//'
}


# Get the LUKS-Device's UUId
function getLUKSDevice(){
  if [ "$1" = "UUID" ]; then
    blkid /dev/sda*[1-9] | grep "crypto_LUKS" | awk '{print $2;}' | sed 's/UUID="//;s/"//'
  fi
  if [ "$1" = "NAME" ]; then
    blkid /dev/sda*[1-9] | grep "crypto_LUKS" | awk '{print $1;}' | sed 's/://'
  fi
}


# Generic dialog question
function ask(){
  dialog --backtitle "$BACKTITLE" --title "$1"  --yesno "$2" 8 80
  return $?
}


# Generic message box
msg() {

  dialog --backtitle "$BACKTITLE" --title "$1" --msgbox "$2" 8 80
}


# Generate a two dimensional flat array of all GPT devices from sdb-sdz
function getMediaDevice(){
  local i=0
  local uuid
  for dev in $(ls /dev/sd[b-z] 2> /dev/null); do
    if $(checkGPT $dev); then
      for part in $(ls $dev*[1-9]); do
        echo -n "$i $part[$(getPARTUUID $part)] "
        (( i++ ))
      done
    fi
  done
}


# Hint for GPT formatted key before searching for it 
function chooseMediumPre(){
  [[ $1 = "Authentication" ]] && { local title="$TITLE_RM_AUTH" ; local insert="$PREWARN_INSERT_AUTH" ; } 
  [[ $1 = "Panic" ]] && { local title="$TITLE_RM_PANIC"; local insert="$PREWARN_INSERT_PANIC" ; }


  dialog --backtitle "$BACKTITLE" --title "$title" --yes-label "$OK" --no-label "$CANCEL" --yesno "$PREWARN_REMOVE\n$PREWARN_GPT\n\n$insert" 20 80
  if [ $? -eq 1 ]; then
    cancel
  fi
}


# Choosing a GPT formatted key
function chooseMedium(){
  local ans 
  [[ $1 = "Authentication" ]] && { local title="$TITLE_RM_AUTH" ; }
  [[ $1 = "Panic" ]] && { local title="$TITLE_RM_PANIC" ; }


  dialog --backtitle "$BACKTITLE" --title "$title" --menu "$CHOOSE_DEV" 10 80 5 $media 2> .pam_panic_media_choice
  if [ $? -eq 1 ]; then
    cancel
  fi

  ans=$(cat .pam_panic_media_choice)
  (( ans=(2*ans)+1 ))
  rm -f .pam_panic_media_choice

  return $ans
}


# A "Detecting devices...", assures to use a more up to date device list
function showDetectDev(){
  dialog --backtitle "$BACKTITLE" \
    --title "$title" \
    --infobox "$DETECT_DEV" 3 80
  # Prevention for impatient beings
  sleep 2
}


# Welcome
dialog --backtitle "$BACKTITLE" \
  --title "$WELCOME" \
  --ok-label "Yip!" \
  --msgbox "$WELCOME1\n\n$WELCOME2\n\n$WELCOME3" 20 80


# Authentication mode
auth_mode=2
while [ $auth_mode -eq 2 ]; do

  dialog --backtitle "$BACKTITLE" \
    --title "$AUTH_MODE" \
    --help-button \
    --extra-button --extra-label "$PASSWORDS" \
    --ok-label "$REM_MEDIA" \
    --yesno "$CHOOSE_AUTH1\n$CHOOSE_AUTH2\n\n$CHOOSE_AUTH3" 10 80

  auth_mode=$?

  case $auth_mode in
    "0")
      # Removable media
      # Authentication
      while [ -z $media ]; do
        chooseMediumPre Authentication
        showDetectDev
        media=$(getMediaDevice)
        read -r -a mediaArray <<< "$media"
      done    
      chooseMedium Authentication
      auth_dev=$(echo ${mediaArray[$?]} | sed 's/\/dev\/sd[b-z]*[0-1]\[//;s/\]//')
      msg "$TITLE_RM_AUTH" "$UUID_AUTH $auth_dev."

      # Panic
      unset media
      while [ -z $media ]; do
        chooseMediumPre Panic
        showDetectDev
        media=$(getMediaDevice)
        read -r -a mediaArray <<< "$media"
      done
      chooseMedium Panic
      panic_dev=$(echo ${mediaArray[$?]} | sed 's/\/dev\/sd[b-z]*[0-1]\[//;s/\]//')
      msg "$TITLE_RM_PANIC" "$UUID_PANIC $panic_dev."

      ;;
    "3")
      # Passwords
      ask "$PASSWORDS" "$ASK_SET_PW"
      setpw=$?
      case $setpw in
          "0")
            clear
            $PAMPANICPW
            if [ $? -ne 0 ]; then
              clear
              echo "$SET_PW_FAILED"
              exit 1
            fi
            ;;
      esac
      ;;
    "2")
      # Help
      man pam_panic
      ;;
    "1")
      # Cancel
      cancel
      ;;
  esac
done


# serious flag
ask "$PAM_PANICS_BEHAVIOUR" "$ASK_SERIOUS1\n$ASK_SERIOUS2"
serious=$?

if [ $serious -eq 0 ]; then
  serious_dev=$(getLUKSDevice UUID)
  if [ ! -z $serious_dev ]; then
    msg "$PAM_PANICS_BEHAVIOUR" "$MSG_SERIOUS1 $(getLUKSDevice NAME) [$serious_dev] $MSG_SERIOUS2"

    # LUKS header backup
    ask "LUKS Header backup" "$ASK_LUKS_BU1\n$ASK_LUKS_BU2 \"$LHBU\"."
    bu=$?
    case $bu in 
      "0")
        cryptsetup luksHeaderBackup $(getLUKSDevice NAME) --header-backup-file "$LHBU"
        msg "LUKS Header backup" "$LUKS_BU_SAVED $LHBU"
        ;;
    esac
  else
    msg "$PAM_PANICS_BEHAVIOUR" "$SERIOUS_ERROR"
    serious=1
  fi
fi


# poweroff / reboot behaviour
dialog --backtitle "$BACKTITLE" \
  --title "$PAM_PANICS_BEHAVIOUR" \
  --ok-label "$REBOOT" \
  --extra-button --extra-label "$SHUTDOWN" \
  --cancel-label "$NOTHING" \
  --yesno "$ASK_EXTENDED_BEHAVIOUR" 10 80
power=$?


# strictness and lockout
ask "$TITLE_STRICT" "$ASK_STRICT"
strict=$?

# Configuration generation
dialog --backtitle "$BACKTITLE" \
  --infobox "$GEN_CONFIG" 3 40
config="#%PAM-1.0\nauth       requisite    $SECUREDIR/pam_panic.so"

case $power in 
  "0")
    config="$config reboot"
    ;;
  "3")
    config="$config poweroff"
    ;;
esac

case $auth_mode in
  "3")
    config="$config password"
    ;;
  "0")
    config="$config allow=$auth_dev reject=$panic_dev"
    ;;
esac

case $serious in
  "0")
    config="$config serious=$serious_dev"
    ;;
esac

case $strict in
  "0")
    config="$config strict"
esac

config="$config\naccount    requisite    $SECUREDIR/pam_panic.so"


# Write config file
writeout=0
if [ -f $CONFIGFILE ]; then
  ask "CONFIG_EXISTS" "$CONFIGFILE $CONFIG_OVERWRITE"
  writeout=$?
  case $writeout in
    "0")
      echo -e "$config" > $CONFIGFILE
      ;;
  esac
else
  echo -e "$config" > $CONFIGFILE
fi


# Finished message
clear
[ $writeout -eq 0 ] && echo "Done! <3" || echo "Nothing done! </3"

echo -e "\n
$WHATNOW"
for (( i=0; i<${#WHATNOW}; i++ )); do
  echo -n "="
done

echo -e "
$SAVEDTO $CONFIGFILE.
$APPLY_THEM
  1. $OPEN_MOD /etc/pam.d/
     $TRY_OUT
     - xscreensaver
     - system-local-login (Arch Linux)
     - common-auth $AND common-account (Ubuntu)
  2. $APPEND
       auth       include    pampanic
       account    include    pampanic
$UBUNTU_NOTICE

$PAMPANIC_ACTIVE
$NEXT_LOGIN
  - $TYPEPW
  - $INSRM
$PREVTOREG

" | more

echo "$QUESTIONS"
echo -e "\n$PRESSENTER"
read -n1
