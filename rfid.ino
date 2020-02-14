//  Project:        Mobile RFID Attendence register 
//                  A project by FireFli (PTY) LTD

//  Date:           October 2019
//  Compiled by:    Friedl Basson
//  Details:        Scan for UID's from RFID cards and posting via webhooks to, UbiDots, mySQL DB or Google Sheets
//                  UPDATE: Display Cardholder name as apposed to UID.                    
//  Credits:        Special thanks to the Particle community for assisting and making this project possible!
//  Firmware:       V2.1.2



//System Mode// - comment out if you will be settng up WiFi via Particle app and do not need to connect to another Wireless network aftwer the fact
//SYSTEM_MODE(SEMI_AUTOMATIC);


// This #include statement was automatically added by the Particle IDE.
#include "RFID.h"
#include <Ubidots.h>


/* Define the pins used for the SS (SDA) and RST (reset) pins for BOTH hardware and software SPI */
#define SS_PIN      A2      
#define RST_PIN     D2

/* Define the pins used for the DATA OUT (MOSI), DATA IN (MISO) and CLOCK (SCK) pins for SOFTWARE SPI ONLY */
#define MOSI_PIN    D3      
#define MISO_PIN    D4      
#define SCK_PIN     D5      


//Initialising RGB START  - RGB LED and buzzer is used as system status indicator
int redPin = D6;       
int greenPin = D1;    
int bluePin = D0;     
int buzzPin = D7;
//Initializing RGB END   


/* Create an instance of the RFID library */
#if defined(_USE_SOFT_SPI_)
    RFID RC522(SS_PIN, RST_PIN, MOSI_PIN, MISO_PIN, SCK_PIN);    // Software SPI
#else
    RFID RC522(SS_PIN, RST_PIN);                                 // Hardware SPI
#endif

//const char* WEBHOOK_NAME = "rfid_uid";
const char* WEBHOOK_NAME = "Ubidots";
Ubidots ubidots("webhook", UBI_PARTICLE);


// List of Card UIDs
char* allowedTags[] = {
"D1 DA 06 20", // Test Tag
"6B 1E 02 1B",
"C0 D3 A1 9F",
"BB 3B 05 1B",
"1B 07 90 C0",
"4B 25 08 1B",
"BB B9 0D 1B",
"0B B2 0B 1B",
"9B 70 10 1B",
"3B 42 0C 1B",
"AB 6B 07 1B",
"5B F3 92 C0",
"CB FA 0C 1B",
"4B 64 7F C1",
"3B 33 06 1B",
"80 F9 A5 9F",
"1B 4E 95 C0",
"8B 36 07 1B",
"7B AE 90 C0",
"9B 1A 05 1B",
"BB AA 2F 32",
"CB AE 8C C0",
"C0 08 A9 9F",
"1B 30 94 C0",
"21 3D 62 64",
"41 22 0F 3A",
"4B A4 01 1B",
"90 B3 A2 9F",
"E0 9E 9F 9F",
"8B B8 96 C0",
"BB A9 8C C0",
"7B DB 07 1B",
"7B BB 8B C0",
"3B E2 89 C0",
"7B EB 07 1B",
"6B 9B 29 32",
"6B 5B 0F 1B",
"9B A4 0D 1B",
"7B 5D 0C 1B",
"BB AE 02 1B",
"AB A7 05 1B",
"FB A2 07 1B",
"FB 95 06 1B",
"DB C2 0D 1B",
"7B 41 0E 1B",
"8B 45 0D 1B",
"CB CE 07 1B",
"4B 7D 0D 1B",
"7B 0D 03 1B",
"EB 84 93 C0",
"0B F7 0E 1B",
"8B D1 01 1B",
"DB C5 96 C0",
"EB 54 05 1B",
"41 EC 10 3A",
"8B 7C 0E 1B",
"3B DA 02 1B",
"CB 71 0F 1B",
"4B 08 09 1B",
"8B 7E 04 1B",
"9B 31 0A 1B",
"AB F0 0E 1B",
"3B C2 06 1B",
"4B 64 0B 1B",
"B0 E6 9B 9F",
"8B D3 08 1B",
"4B E8 05 1B",
"DB DA 09 1B",
"7B E2 02 1B",
"DB 69 0C 1B",
"5B FA 0F 1B",
"E1 E0 0B 3A",
"4B 0A 7F C1",
"FB 33 0F 1B",
"FB 12 0C 1B",
"2B 58 03 1B",
"4B 7B 0B 1B",
"2B 2A 0D 1B",
"6B 62 02 1B",
"3B CD 0D 1B",
"FB 18 09 1B",
"4B AF 0D 1B",
"1B AB 02 1B",
"BB 2A 09 1B",
"7B 83 07 1B",
"BB 20 10 1B",
"CB 7F 07 1B",
"9B 31 06 1B",
"4B 58 0E 1B",
"8B 6D 04 1B",
"9B 9B 05 1B",
"3B 5C 0F 1B",
"FB 3F 06 1B",
"1B 47 10 1B",
"AB E1 0B 1B",
"CB B6 95 C0",
"8B 81 91 C0",
"1B F1 8B C0",
"AB B7 09 1B",
"BB 68 06 1B",
"1B A9 01 1B",
"2B 78 01 1B",
"EB BD 06 1B",
"CB 7D 0E 1B",
"1B F2 0E 1B",
"E1 97 0D 3A",
"2B CC 0D 1B",
"FB 78 8C C0",
"9B BD 0E 1B",
"BB 57 02 1B",
"DB 6C 0D 1B",
"5B 95 0B 1B",
"DB 54 03 1B",
"DB 6D 09 1B",
"4B F1 8B C0",
"9B 57 0F 1B",
"3B 40 93 C0",
"0B 98 08 1B",
"4B F3 0E 1B",
"6B AF 02 1B",
"CB 9A 02 1B",
"CB 66 05 1B",
"7B DC 04 1B",
"9B 8D 10 1B",
"EB 7D 10 1B",
"AB CD 02 1B",
"4B E4 06 1B",
"1B 3C 0C 1B",
"4B 22 0B 1B",
"9B 8F 0D 1B",
"51 B4 11 3A",
"EB E4 8D C0",
"CB 50 0E 1B",
"7B 1F 05 1B",
"2B E3 01 1B",
"DB AE 10 1B",
"AB 88 0A 1B",
"7B A5 03 1B",
"0B F3 09 1B",
"AB 8E 0C 1B",
"5B A8 90 C0",
"6B 77 02 1B",
"6B 13 02 1B",
"EB 0A 06 1B",
"2B D6 09 1B", 
"7B EE 06 1B",
"AB 78 09 1B",
"3B BF 0A 1B",
"CB BF 0C 1B",
"2B B7 07 1B",
"5B 03 10 1B",
"4B 77 07 1B",
"6B E8 08 1B",
"AB 26 04 1B",
"7B 64 0B 1B",
"5B 77 0E 1B",
"6B B1 02 1B",
"DB 11 03 1B",
"CB 26 09 1B",
"FB 4E 95 C0",
"9B CB 06 1B",
"9B 67 02 1B",
"1B 94 0B 1B",
"7B D0 06 1B",
"9B 60 07 1B",
"5B 5F 0D 1B",
"2B 02 02 1B",
"6B 29 07 1B",
"CB 66 04 1B",
"3B 15 0E 1B",
"6B 4F 94 C0",
"AB 6E 0B 1B",
"1B 40 10 1B",
"AB E4 0E 1B",
"4B E0 05 1B",
"0B 26 10 1B",
"0B 9A 02 1B",
"1B D1 08 1B",
"6B 9E 0C 1B",
"8B 8F 0D 1B",
"6B 22 8C C0",
"CB 84 8B C0",
"AB B4 8A C0",
"AB D4 0B 1B",
"4B 07 8D C0",
"8B 68 10 1B",
"5B AD 8A C0",
"5B 04 04 1B",
"9B AB 7B C1",
"DB 36 91 C0",
"CB 09 94 C0",
"2B B6 08 1B",
"BB 5A 0A 1B",
"3B 25 0D 1B",
"7B CE 0A 1B",
"1B 88 07 1B",
"1B 57 96 C0",
"4B 44 06 1B",
"DB C4 0F 1B",
"2B 4D 0E 1B",
"3B AF 0E 1B",
"DB 78 0B 1B",
"1B 23 8C C0",
"DB D0 94 C0",
"9B A4 90 C0",
"0B 8B 8D C0",
"9B 0F 97 C0",
"BB 43 89 C0",
"AB DA 95 C0",
"AB EC 90 C0",
"3B A1 89 C0",
"AB 68 7C C1",
"5B 74 91 C0",
"8B 8A 92 C0",
"DB 18 10 1B",
"3B 04 96 C0",
"5B 4A 93 C0",
"7B B8 94 C0",
"FB 0F 0F 1B",
"DB 1E 7E C1",
"AB CD 7B C1",
"AB 40 96 C0",
"EB C8 92 C0",
"8B 5E 97 C0",
"2B 5C 93 C0",
"7B 30 92 C0",
"FB 99 8A C0",
"EB 8D 8F C0",
"3B DD 88 C0",
"AB 7B 91 C0",
"0B 8B 95 C0",
"6B 60 96 C0",
"0B 0F 8A C0",
"4B F7 91 C0",
"7B 60 97 C0",
"FB 70 89 C0",
"EB 29 7E C1",
"BB 49 90 C0",
"1B BC 90 C0",
"1B 8B 7E C1",
"9B CC 95 C0",
"AB 08 81 C1",
"1B 0B 90 C0",
"7B 46 91 C0",
"5B 26 8C C0",
"0B 6E 82 C1",
"FB EE 95 C0",
"BB EC 90 C0",
"4B 63 0C 1B",
"EB 6F 90 C0",
"7B 2B 96 C0",
"8B 16 95 C0",
"6B 2E 91 C0",
"9B CE 07 1B",
"8B 74 8A C0",
"DB A7 90 C0",
"AB 16 90 C0",
"1B 27 8C C0",
"9B D7 8A C0",
"DB 8F 8F C0",
"7B 54 8B C0",
"AB 91 96 C0",
"4B 8B 7C C1",
"9B 70 89 C0",
"4B 77 97 C0",
"3B BE 92 C0",
"EB 74 89 C0",
"8B 46 90 C0",
"AB D0 94 C0",
"8B C6 94 C0",
"4B 84 90 C0",
"5B 03 94 C0",
"BB 3C 91 C0",
"1B 74 95 C0",
"2B 87 89 C0",
"1B 91 03 1B",
"4B 3A 96 C0",
"EB FD 7F C1",
"7B 77 90 C0",
"2B 8F 92 C0",
"5B 33 91 C0",
"1B 09 93 C0",
"2B BA 96 C0",
"AB 7B 89 C0",
"5B 09 8E C0",
"EB B5 8A C0",
"8B A9 8C C0",
"6B AF 92 C0",
"6B 2C 92 C0",
"8B 95 8F C0",
"6B 20 97 C0",
"10 C9 A0 9F",
"0B C6 0C 1B",
"81 5D 07 3A",
 };

// List of names to associate with the matching tag IDs
 char* tagName[] = {
"Friedl", // Test Tag 
"LUKE LALIN",
"JACQUES JORDAAN",
"GUEST SUPPLIER",
"DANIELLA FLAY",
"BARRY LANGENHOVEN",
"CHRIS ALLFLATT",
"PEGGY BZINKIEWICZ",
"LETICIA MACHADO",
"SIAN BESTER",
"VISITOR 11",
"HESTER BOTES",
"VISITOR 12",
"RUSHANA CASSIEM",
"VISITOR 13",
"JAUN JORDAAN",
"ELIZABETH MANSFIELD",
"TRACEY MAJOLA",
"NICHOLAS LUKE JOHNSON",
"SEAN BROOKES",
"RIC CUTINO",
"IRINA JORDAAN",
"MICHELLE PETRUS",
"MICHELLE WELVERING",
"LINDSAY WITKOWSKY",
"RICARDO SANCHEZ",
"MURRAY MACLARTY",
"FRANCIS CHITSANZARA",
"LAWRENCE MASIZA",
"VISITOR",
"VISITOR 02",
"VISITOR 03",
"VISITOR 03",
"NICOLETTE GAERTNER",
"VISITOR 09",
"LUKE COETZER",
"NAKITA BAM",
"UNATHI GUMA",
"KEVIN MBUISHA",
"JUSTINE RYAN",
"NOSIPHO DYASI",
"GERSHWIN ALLIE",
"ABEL RIOS",
"ITUMELENG MALATSI",
"LEE BERGMAN",
"KULSUM BASARDIEN",
"GARETH DE JAGER",
"GLEN ALBRECHT",
"ANOUK RENNER",
"TARA BECKETT",
"KIMBERLEY SMITH",
"VALINA FIRNEISEN",
"LEANDRE LIEDEMANN",
"TERRI-ANN SCHOLTZ",
"SARAH DELFORGE",
"YINETTE BRACHO",
"KAGISO SEGANO",
"ANANDRE ELS",
"TANYA HOLTZHAUSEN",
"STUART LEWIS",
"MAI UMBHAU",
"WILMONT KLAAS",
"JULIEN GHIGHI",
"KATHARINA RIEBESEL",
"SETON FISHER",
"OLUFEMI JOHNSON",
"YOLANDA BARON",
"DANNIE DU PLESSIS",
"SHAMEMA FAULMAN",
"WANITA KATZ",
"TATIANA ZAMBRANO",
"KHAYA MAYEKISO",
"NOLUTHANDO NGALO",
"LAURA SLABBER",
"LORI KIETZMANN",
"SIMON WATSON",
"TIAAN VAN AARDT",
"HARDY ENGELBRECHT",
"WALEED BAX",
"RON SOLOMONS",
"PORTIA MKOSI",
"JULIA HARKER",
"CARLO DU PLESSIS",
"CANDICE FEBRUARY",
"MANUELA WILFART",
"ANJA KEMP",
"JANINE GOUS",
"DANIELA FRASER",
"SASKIA VAN DALEN",
"MATTHEW SMITH",
"LESLEY FOSTER",
"ZANDILE KOSE",
"AMARILIO NETO",
"SAMANTHA MYBURGH",
"LISIWE QUKWANA",
"LYDIA HLANGANYANA",
"JEFFREY RUSCH",
"KELLY MCCARTY",
"ALEXANDRA BARTH",
"HELEN KIRBY",
"MIRIAM JIMENEZ GARCIA",
"ELIZABETH NKOANA",
"ANNA MARTORELL",
"KANYISA SHUDE",
"LETIZIA LOPEZ",
"LESLEY HAGEMANN",
"KRISTY STEYN",
"TREVOR GOWING",
"CARL PRELLER",
"BIANCA JOHNSTONE",
"CELESTE LE ROUX",
"BRITTA FRANKE",
"GILLIAN MEYER",
"IAN KRIEK",
"Andisile Mbenyane",
"ADICK RENNER",
"ALLISON BOTHMA",
"MARLIN JACKSON",
"GUDANI BVUMBI",
"MIA DE VILLERS",
"MARIO MONROY",
"TOAST SEAGERS",
"BJORN DIJKSHOORN",
"JULIA GRASS",
"PELEKA MGUGUDO",
"HILTON WARMBACK",
"BRONWYNNE DU TOIT",
"CANDICE MCCARTHY",
"SHIRLEY PONOSHE",
"FAZLIN FLORIS",
"CAMILLE FOLO",
"DONOVAN PROUDFOOT",
"ELLEANOR SHA",
"LANDY KENT-MILLAR",
"KERUSHA NAIDOO",
"BILLY HARE",
"DESIRÉ SAAYMAN",
"QUINTIN STOLTZ",
"GALEN SCHULTZ",
"GRACIA FREIRE",
"TERENCE RYAN",
"ISABEAU VAN DER EECKEN",
"DAVID RYAN",
"NAZEER ERASMUS",
"MANSOER SOLOMON",
"TUBBY LUBOBO",
"EVELYN CHIBWE",
"PATTI BROCKMANN",
"GRANT RAPAPORT",
"ANKE GIESE",
"VISITOR 15",
"VISITOR 18",
"JUSTINE RYAN",
"JO MAREE",
"FELIPE BROSOU BONAHORO",
"LAEEKA FAULMAN",
"DENUSHA BLUNDER",
"VISITOR 19",
"VISITOR 05",
"TANIA DE KOCK",
"ANDREA RAMOS",
"TAAHIR ISAACS",
"TYRONE FISHER",
"ARTHUR PRICE",
"VISITOR 04",
"KAREN ENGELBRECHT",
"MATTHEW RYAN",
"BRONTE MILES",
"TONY ROMER-LEE",
"LAUREN GREEN",
"KIM CAMERON",
"GLENN GILLIS",
"NICCI GOWING",
"WYNAND GROENEWALD",
"LEBO LEKOMA",
"JADE MATHIESON",
"RHODA RUTHERFORD",
"TECH TECH",
"LEA PICHOUD",
"JOHANNES ACKERMAN",
"CASSANDRA BETTINI",
"MEGAN BROPHY",
"JASON BURROWS",
"LARA DE GROOT",
"YNZE DE JONG",
"MALAN DE KOCK",
"HERMAN DE VILLIERS",
"MUNKI GROENEWALD",
"NIC HOEKSTRA",
"RODAIN JOUBERT",
"ABIGAIL KWAN",
"CHESLYN MOSES",
"KATE LEFKO-EVERETT",
"MEGAN LENTIN",
"ROMI LOCKITCH",
"LUKE MACKAY",
"KHAALIED MAJIET",
"RISHAD OMAR",
"KANYISA KALIPA",
"FRANCOIS MALHERBE",
"CHADLEY MULLER",
"NEIL PHYFER",
"VERASTHA RAMANJAM-GAUNT",
"GATHREY REEDING",
"MICHAEL SAMUELS",
"LUCAS SWART",
"KYLE VAN VUUREN",
"JONETTE COETZEE",
"DIRK PILON",
"CAROLINE WESSIE-BOGOSHI",
"CHESLIN MERT",
"ROZANNE DE JAGER",
"MUHAMMAD SEEDAT",
"KISHEN PATEL",
"QUINTON STOLTZ",
"WAATHIQ HILTON",
"STEPHANIE SCHWATLO",
"TRAVIS TANSLEY",
"CRISTINA ARMESTO",
"BRETT AUSMEIER",
"VISITOR 01-SM",
"DANDO GUEST",
"DARYLL WILLIAMS",
"SUSAN BROWN",
"JANET SMITH",
"AMY HUNTER",
"KELSEY PETERSEN",
"THABANG MACHONA",
"VISITOR 02-SM",
"VISITOR 03-SM",
"VISITOR 04-SM",
"VISITOR 05-SM",
"TEGAN KELLY",
"CAYLEY BAKER",
"CASSANDRA TOMLINSON",
"SIVUYILE DZONO",
"PATRICIA SANTOS",
"KELLY DREYER",
"JURGEN BORNMAN",
"TARA GOMM",
"MARK KLEYNHANS",
"DAVE LINDER",
"MARTIN McCABE",
"BILLY NEUMANN",
"DURK POSTMA",
"BLESSING SACHIKONYE",
"LUKE MARSH",
"MICHELA TROLESE",
"WAYDE",
"CAYLEIGH BROWN",
"BRAD MITCHELL",
"SIMONE SAMUELS",
"ZULEIGA ADAMS",
"ASANDA MZAYIDUME",
"STEFANIE RUFF",
"JOGI NIJJAR",
"LINDA SIBOTO",
"SHIKARA NEL",
"RIAAZ KISTEN",
"KEVIN JANSEN",
"VISITOR 02",
"VISITOR 06",
"VISITOR 01",
"VISITOR 07",
"VISITOR 08",
"VISITOR 10",
"VISITOR 14",
"VISITOR 16",
"VISITOR 17",
"VISITOR 20",
"Msimelelo Komisa",
"Vuyani Lisana",
"Zamuxolo Mayeku",
"IAN MARTIN",
"Luyanda Mdudu",
"LENA HABERSATTER",
"Kyoni Minda",
"Claudia Spicciati",
"TESTER tester",
"CHARLES SIBOTO",
"Alistena Gerstner",
"Nicci Acker",
"TERRI LOURENS",
"ROWENA PIENAAR",
"SIMON TOUA",
"MAXIM TSHIBANGU",
"TATIANA SKLIARENKO",
"RAYMONITA ROSSOUW",
"Joshua Smith",
"ENOS MAKUMBE",
"SIBONGILE NGALO",
"ZOLELA MASIZA",
"MONIQUE VALLY",
"CHARL LOTTER",
 };



String str;
char tagValue[16];

int numberOfTags = 294; //Total number of tags used

void setup()
    {
         Serial.begin(9600);    
  
//  WiFi.listen();

        pinMode(redPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        pinMode(bluePin, OUTPUT);
        pinMode(buzzPin, OUTPUT);
    

// Setting credentials for new WiFi Connection
/*  
    This section is only needed if you want to connect to a new WiFi Network after initial setup
    This will only work if current WiFi credentials have been cleared.  
    Code will run ONLY ONCE on startup and set new crediatials specified.  Once new credentials is set, this code will not run again.
*/

WiFi.on();

if (!WiFi.hasCredentials()) {
WiFi.setCredentials("Rhino Africa Office", "Pang01in", WPA2, WLAN_CIPHER_AES);      //Rhino Africa Office//Pang01in//WPA2/WLAN_CIPHER_AES
}

WiFi.connect();
waitUntil(WiFi.ready);
Particle.connect();

  
  
#if !defined(_USE_SOFT_SPI_)
  /* Enable the HW SPI interface */
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.begin();
#endif

  /* Initialise the RFID reader */
  RC522.init();

delay(5000);
            
            digitalWrite(bluePin, HIGH);
            digitalWrite(greenPin, HIGH);
            digitalWrite(redPin, LOW); 
}

void loop()

{
  uint8_t i;

  /* Has a card been detected? */
  if (RC522.isCard())
  {
    /* If so then get its serial number */
    RC522.readCardSerial();

  char CID[16]; 
  snprintf(CID, sizeof(CID)
          , "%02X %02X %02X %02X"
          , RC522.serNum[0]
          , RC522.serNum[1]
          , RC522.serNum[2]
          , RC522.serNum[3]
          );

    int iPos = findTag(CID);
    
    

// Webhooks//
/*
RFID_WP = your webhook name
str = your data you want to publish
PRIVATE or PUBLIC
*/

//            Particle.publish("RFID_WP", str, PRIVATE);          //Publish Data to Wordpress DB
//            Particle.publish("XLS", str, PRIVATE);              // Publish Data to Google Sheets   


// /* Adds context key-value pairs */
    if (iPos >= 0)
    {
        ubidots.addContext("UID", tagName[iPos]);  //str  //tagValue
    }
    else
    {
        ubidots.addContext("UID", CID);  //str  //tagValue
    }
    char* context = (char *) malloc(sizeof(char) * 60);

    /* Builds the context with the array above to send to Ubidots */
    ubidots.getContext(context);
    ubidots.add("UID", 1, context);  // Change for your variable name  
    
    bool bufferSent = false;
    bufferSent = ubidots.send(WEBHOOK_NAME, PUBLIC);  // Will use particle webhooks to send data
    
            delay(500);
            digitalWrite(buzzPin, LOW);
            digitalWrite(bluePin, LOW);
            delay(750);
            digitalWrite(bluePin, HIGH);
 
  }          
 
 else if (RC522.isCard())
  
  {

    RC522.readCardSerial();

 char CID[16]; 
 snprintf(CID, sizeof(CID)
          , "%02X %02X %02X %02X"
          , RC522.serNum[0]
          , RC522.serNum[1]
          , RC522.serNum[2]
          , RC522.serNum[3]
          ); 

            digitalWrite(bluePin, LOW);
            digitalWrite(greenPin, LOW);
            digitalWrite(redPin, HIGH);
            delay(1000);
            digitalWrite(bluePin, HIGH);
            digitalWrite(greenPin, HIGH);
            digitalWrite(redPin, LOW);
            digitalWrite(buzzPin, LOW);
    }

  delay(500);
}

int findTag(char tagValue[16]) 
{
    for (int thisCard = 0; thisCard < numberOfTags; thisCard++) {
// Check if the tag value matches this row in the tag database
        if(strcmp(tagValue, allowedTags[thisCard]) == 0)
        {
// The row in the database starts at 0, so add 1 to the result so
// that the card ID starts from 1 instead (0 represents "no match")
            return(thisCard);
        }
    }
// If we don't find the tag return a tag ID of 0 to show there was no match
return -1;
}