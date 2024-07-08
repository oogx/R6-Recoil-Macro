#include "Drawing.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <random>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
LPCSTR Drawing::lpWindowName = "Silent Solutions";
ImVec2 Drawing::vWindowSize = { 600, 500 };
ImGuiWindowFlags Drawing::WindowFlags = 0;
bool Drawing::bDraw = true;
UI::WindowItem Drawing::lpSelectedWindow = { nullptr, "", "" };
// Recoil Aruino
int sensitivity = 1;
bool Recoil_Enabled = false;
int Recoil_Sensitivity = 1;
float Recoil_X = 0.0;
float Recoil_Y = 0.0;
// Recoil Math
float PullDownPixels = 9;
int FOV = 90;
double PreSetHorizontal = 12;
double PreSetVertical = 12;

double Dpi = 3200;

int NewHorizontal = 12;
int NewVertical = 12;
int X1_5 = 42;
int X2_0 = 42;
int X2_5 = 42;
int X3_0 = 42;

// Ui Settings
// Recoil Settings
static const char* SightList[] = { "None", "Non Magnifying", "1.5x Scope", "2.0x Scope", "2.5x Scope", "3.0x Scope" };
static const char* GripList[] = { "None", "Vertical", "Angled" };
static const char* AttachmentList[] = { "None", "Flash Hider", "Compensator", "Muzzle Brake", "Suppressor", "Extended Barrel" };
static const char* SelectedSight = SightList[0];
static const char* SelectedGrip = GripList[0];
static const char* SelectedAttachment = AttachmentList[0];
static const char* Current_Side = "";
static const char* Current_Weapon_Slot = "";
static std::string Attacker_Name = "";
static std::string Attacker_Primary = "";
static std::string Attacker_Secondary = "";
static const char* Attacker_Primary_Sight = "";
static const char* Attacker_Primary_Grip = "";
static const char* Attacker_Primary_Attachment = "";
static const char* Attacker_Secondary_Sight = "";
static const char* Attacker_Secondary_Grip = "";
static const char* Attacker_Secondary_Attachment = "";
static std::string Defender_Name = "";
static std::string Defender_Primary = "";
static std::string Defender_Secondary = "";
static const char* Defender_Primary_Sight = "";
static const char* Defender_Primary_Grip = "";
static const char* Defender_Primary_Attachment = "";
static const char* Defender_Secondary_Sight = "";
static const char* Defender_Secondary_Grip = "";
static const char* Defender_Secondary_Attachment = "";
float Move_X_Value, Move_Y_Value;
//Randomisation
bool Randomisation_Enable = true;
float Randomisation_X_Slider = 0.020;
float Randomisation_Y_Slider = 0.020;

float Randomisation_X_Value = 0;
float Randomisation_Y_Value = 0;
// Watermark
bool Watermark_Enabled = false;
std::string Watermark_Text = "";
const char* Watermark_Weapon = "";
const char* Watermark_Sight = "";
const char* Watermark_Attachment = "";
const char* Watermark_Grip = "";
std::string Cheat_Text = "";
const char* Cheat_Name = "Silent Solutions";
const char* Cheat_Game = "Rainbow Six Siege";

double AdjustedPulldownValue, Sightpulldown, PulldownValue, CurrentSight, CurrentGrip, CurrentAttachment;
struct Weapon {
    std::string Weapon_Name;
    std::vector<std::string> X;
    std::vector<std::string> Y;
};
struct Attacker {
    std::string Operator_Name;
    std::vector<std::string> Primary_Weapons;
    std::vector<std::string> Secondary_Weapons;
};
struct Defender {
    std::string Operator_Name;
    std::vector<std::string> Primary_Weapons;
    std::vector<std::string> Secondary_Weapons;
};
std::vector<Weapon> Weapons{
    {"L85A2", {"1.374"}, {"6.587"}},
    {"M590A1", {"0"}, {"0"}},
    {"P226 MK 25", {"0.895"}, {"3.927"}},
    {"AR33", {"-0.62"}, {"7.267"}},
    {"G36C", {"1.107"}, {"9"}},
    {"R4-C", {"-1.4"}, {"10.3"}},
    {"M45 Meusoc", {"0"}, {"10.3"}},
    {"M1014", {"0"}, {"0"}},
    {"556XI", {"1.08"}, {"6.338"}},
    {"F2", {"-1.128"}, {"12.022"}},
    {"417", {"0.647"}, {"6.965"}},
    {"SG-CQB", {"0"}, {"0"}},
    {"P9", {"0.966"}, {"4.493"}},
    {"LFP586", {"-1.207"}, {"23.242"}},
    {"OTs-03", {"0.966"}, {"13.16"}},
    {"OTs-03 Thermal Scope", {"0.966"}, {"40"}},
    {"PMM", {"0"}, {"10.888"}},
    {"GONNE-6", {"0"}, {"0"}},
    {"Bearing 9", {"-1.108"}, {"11.385"}},
    {"BALLISTIC SHIELD", {"0"}, {"0"}},
    {"6P41", {"-1.268"}, {"12"}},
    {"AK-12", {"-1.008"}, {"8.976"}},
    {"G52-TACTICAL SHIELD", {"0"}, {"0"}},
    {"P12", {"0.837"}, {"6.826"}},
    {"AUG A2", {"1.038"}, {"8.235"}},
    {"552 Commando", {"-0.908"}, {"7.266"}},
    {"G8A1", {"-1.269"}, {"9.965"}},
    {"C8-SFW", {"-0.058"}, {"7.7455"}},
    {"CAMRS", {"-0.058"}, {"7.905"}},
    {"MK1 9mm", {"0.873"}, {"6.981"}},
    {"MK17 CQB", {"1.110"}, {"4.488"}},
    {"SR-25", {"-0.567"}, {"13.514"}},
    {"D-50", {"-0.567"}, {"25.944"}},
    {"PARA-308", {"-0.809"}, {"6.7"}},
    {"M249", {"-0.685"}, {"7.866"}},
    {"PRB92", {"0.474"}, {"5.021"}},
    {"TYPE-89", {"-1.046"}, {"8.739"}},
    {"SUPERNOVA", {"0"}, {"0"}},
    {"P229", {"0.821"}, {"7.203"}},
    {"PDW9", {"0.821"}, {"7.944"}},
    {"C7E", {"0.821"}, {"9.924"}},
    {"ITA12L", {"0"}, {"0"}},
    {"USP40", {"0.821"}, {"5.224"}},
    {"ITA12S", {"0"}, {"0"}},
    {"T-95 LSW", {"0.821"}, {"7.2"}},
    {"SIX12", {"0"}, {"0"}},
    {"Q-929", {"-0.143"}, {"11.371"}},
    {"M762", {"0.821"}, {"9.021"}},
    {"LMG-E", {"-1.429"}, {"11"}},
    {"RG15", {"0.821"}, {"7"}},
    {"Mk 14 EBR", {"-1.106"}, {"6.934"}},
    {"BOSG.12.2", {"0"}, {"0"}},
    {"SMG-12", {"1.240"}, {"15.478"}},
    {"C71 Auto", {"0.972"}, {"8.845"}},
    {"V308", {"0.421"}, {"6.987"}},
    {"Spear .308", {"-1.195"}, {"9.8"}},
    {"SASG-12", {"-2.4"}, {"42"}},
    {"AR-15.50", {"-0.797"}, {"8.685"}},
    {"M4", {"-1.036"}, {"8.328"}},
    {"1911 Tacops", {"0"}, {"10.130"}},
    {"AK-74M", {"-1"}, {"5.789"}},
    {"ARX200", {"-1.017"}, {"7.288"}},
    {".44 Mag Semi-Auto", {"6.576"}, {"64.407"}},
    {"F90", {"-1"}, {"8.111"}},
    {"M249 SAW", {"1"}, {"7.464"}},
    {"SDP 9mm", {"0"}, {"11.037"}},
    {"FMG-9", {"0"}, {"7.9"}},
    {"SIX12 SD", {"0"}, {"0"}},
    {"SMG-11", {"-0.907"}, {"14"}},
    {"CSRX 300", {"0"}, {"0"}},
    {"SPSMG9", {"0.381"}, {"7.261"}},
    {"SC3000K", {"-1"}, {"9.92"}},
    {"MP7", {"0.352"}, {"7.9"}},
    {"5.7 USG Special", {"0.804"}, {"6.930"}},
    {"POF-9", {"-0.272"}, {"7.934"}},
    {"MP5K", {"-0.272"}, {"7.860"}},
    {"UMP45", {"-0.955"}, {"4.9"}},
    {"MP5", {"-0.826"}, {"8.5"}},
    {"P90", {"-0.826"}, {"7.421"}},
    {"9x19VSN", {"-0.981"}, {"8.571"}},
    {"DP27", {"1.451"}, {"3.028"}},
    {"M870", {"0"}, {"0"}},
    {"416-C Carbine", {"-1.136"}, {"10.810"}},
    {"Super 90", {"0"}, {"0"}},
    {"9mm C1", {"0"}, {"4.9"}},
    {"MPX", {"-0.840"}, {"7.9"}},
    {"SPAS-12", {"0"}, {"0"}},
    {"M12", {"-0.998"}, {"4.919"}},
    {"SPAS-15", {"0"}, {"0"}},
    {"LUISON", {"1.322"}, {"9.794"}},
    {"MP5SD", {"-0.976"}, {"7.627"}},
    {"Vector .45 ACP", {"0.978"}, {"11.176"}},
    {"T-5 SMG", {"0"}, {"9.328"}},
    {"Scorpion Evo 3 A1", {"2.134"}, {"11.555"}},
    {"FO-12", {"-1.769"}, {"42.5"}},
    {"K1A", {"-1.009"}, {"8.0"}},
    {"ALDA 5.56", {"0"}, {"10.90"}},
    {"ACS12", {"-0.992"}, {"3.885"}},
    {"Keratos .357", {"0"}, {"20.45"}},
    {"Mx4 Storm", {"0.696"}, {"9.71"}},
    {"CCE SHIELD", {"0"}, {"0"}},
    {"P-10C", {"0.482"}, {"6.746"}},
    {"AUG A3", {"0.783"}, {"7"}},
    {"TCSG12", {"0.783"}, {"15.693"}},
    {"Commando 9", {"-0.39"}, {"7.1"}},
    {"P10 Roni", {"1.628"}, {"8.961"}},
    {"UZK50GI", {"-0.368"}, {"8.344"}},
};
std::vector<Attacker> Attackers{
    // SAS
    {"Sledge", {"L85A2", "M590A1"}, {"P226 MK 25"}},
    {"Thatcher", {"AR33","L85A2", "M590A1"}, {"P226 MK 25"}},
    // FBI Swat
    {"Ash", {"G36C", "R4-C"}, {"5.7 USG", "M45 Meusoc"}},
    {"Thermite", {"M10914", "556XI"}, {"5.7 USG", "M45 Meusoc"}},
    // GIGN
    {"Twitch", {"F2", "417", "SG-CQB"}, {"P9", "LFP586"}},
    {"Montagne", {"LE ROC SHIELD"}, {"P9", "LFP586"}},
    // Spetsnaz
    {"Glaz", {"OTs-03", "OTs-03 Thermal Scope"}, {"PMM", "GONNE-6", "Bearing 9"}},
    {"Fuze", {"BALLISTIC SHIELD", "6P41", "AK-12"}, {"PMM", "GSH-18"}},
    // GSG 9
    {"Blitz", {"G52-TACTICAL SHIELD"}, {"P12"}},
    {"IQ", {"AUG A2", "552 Commando", "G8A1"}, {"P12"}},
    // JTF2
    {"Buck", {"C8-SFW", "CAMRS"}, {"MK1 9mm", "GONNE-6"}},
    // Navy Seal
    {"BlackBeard", {"MK17 CQB", "SR-25"}, {"D-50"}},
    // Bope
    {"Capitao", {"PARA-308", "M249"}, {"PRB92", "GONNE-6"}},
    // SAT
    {"Hibana", {"TYPE-89", "SUPERNOVA"}, {"P229", "Bearing 9"}},
    // GEO
    {"Jackal", {"C7E", "PDW9", "ITA12L"}, {"USP40", "ITA12S"}},
    // SDU
    {"Ying", {"T-95 LSW", "SIX12"}, {"Q-929"}},
    // Grom
    {"Zofia", {"LMG-E", "M762"}, {"RG15"}},
    // 707th SMB
    {"Dokkaebi", {"Mk 14 EBR", "BOSG.12.2"}, {"SMG-12", "C75 Auto", "GONNE-6"}},
    // CBRN Threat Unit
    {"Lion", {"V308", "417", "SG-CQB"}, {"LFP586", "P9"}},
    {"Finka", {"Spear .308", "6P41", "SASG-12"}, {"PMM", "GSH-18"}},
    // GSUTR
    {"Maverick", {"AR-15.50", "M4"}, {"1911 Tacops"}},
    // GIGR
    {"Nomad", {"AK-74M", "ARX200"}, {".44 Mag Semi-Auto", "PRB92"}},
    // SASR
    {"Gridlock", {"F90", "M249 SAW"}, {"Super Shorty", "SDP 9mm"}},
    // Jaeger Corps
    {"Nokk", {"FMG-9", "SIX12 SD"}, {"5.7 USG", "D-50"}},
    // APCA
    {"Amaru", {"G8A1", "SUPERNOVA"}, {"SMG-11", "ITA12S", "GONNE-6"}},
    // Nighthaven
    {"Kali", {"CSRX 300"}, {"SPSMG9", "C75 Auto", "P226 MK 25"}},
    // REU
    {"Iana", {"ARX200", "G36C"}, {"MK1 9mm", "GONNE-6"}},
    // Nighthaven
    {"Ace", {"AK-12", "M1014"}, {"P9"}},
    // ROS
    {"Zero", {"SC3000K", "MP7"}, {"5.7 USG Special", "GONNE-6"}},
    // Unaffiliated
    {"Flores", {"AR33", "SR-25"}, {"GSH-18"}},
    // Nighthaven
    {"Osa", {"556XI", "PDW9"}, {"PMM"}},
    // SFG
    {"Sens", {"POF-9", "417"}, {"SDP 9mm"}},
    // Nighthaven
    {"Grim", {"552 Commmando", "SG-CQB"}, {"P229", "Bailiff 410"}},
    // COT
    {"Brava", {"PARA-308", "CAMRS"}, {"Super Shorty", "USP40"}},
    // 35TH Commando Battalion
    {"Ram", {"R4-C", "LMG-E"}, {"MK1 9mm", "ITA12S"}},
};
std::vector<Defender> Defenders{
    // SAS
    {"Smoke", {"FMG-9", "M590A1"}, {"P226 MK 25", "SMG-11"}},
    {"Mute", {"MP5K", "M590A1"}, {"P226 MK 25", "SMG-11"}},
    // FBI Swat
    {"Castle", {"UMP45", "M1014"}, {"5.7 USG", "Super Shorty", "M45 Meusoc"}},
    {"Pulse", {"UMP45", "M1014"}, {"5.7 USG", "M45 Meusoc"}},
    // GIGN
    {"Doc", {"SG-CQB", "MP5", "P90"}, {"P9", "LFP586", "Bailiff 410"}},
    {"Rook", {"SG-CQB", "MP5", "P90"}, {"P9", "LFP586"}},
    // Spetsnaz
    {"Kapkan", {"9x19VSN", "SASG-12"}, {"PMM", "GSH-18"}},
    {"Tachanka", {"9x19VSN", "DP27"}, {"PMM", "GSH-18", "Bearing 9"}},
    // GSG 9
    {"Jager", {"M870", "416-C Carbine"}, {"P12"}},
    {"Bandit", {"MP7", "M870"}, {"P12"}},
    // JTF2
    {"Frost", {"Super 90", "9mm C1"}, {"MK1 9mm", "ITA12S"}},
    // Navy Seal
    {"Valkyrie", {"SPAS-12", "MPX"}, {"D-50"}},
    // Bope
    {"Caveira", {"M12", "SPAS-15"}, {"LUISON"}},
    // SAT
    {"Echo", {"SUPERNOVA", "MP5SD"}, {"P229", "Bearing 9"}},
    // GEO
    {"Mira", {"Vector .45 ACP", "ITA12L"}, {"USP40", "ITA12S"}},
    // SDU
    {"Lesion", {"SIX12 SD", "T-5 SMG"}, {"Q-929", "Super Shorty"}},
    // GROM
    {"Ela", {"Scorpion Evo 3 A1", "FO-12"}, {"RG15"}},
    // 707th SMB
    {"Vigil", {"K1A", "BOSG.12.2"}, {"C75", "SMG-12"}},
    // GIS
    {"Maestro", {"ALDA 5.56", "ACS12"}, {"Bailiff 410", "Keratos .357"}},
    {"Alibi", {"Mx4 Storm", "ACS12"}, {"Bailiff 410", "Keratos .357"}},
    // GSUTR
    {"Clash", {"CCE Shield"}, {"Super Shorty", "SPSMG9", "P-10C"}},
    // GIGR
    {"Kaid", {"AUG A3", "TCSG12"}, {".44 Mag Semi-Auto", "LFP586"}},
    // SASR
    {"Mozzie", {"Commando 9", "P10 Roni"}, {"SDP 9mm"}},
    // Secret Service
    {"Warden", {"M590A1", "MPX"}, {"P-10C", "SMG-12"}},
    // Fuerzas Especiales
    {"Goyo", {"Vector .45 ACP", "TCSG12"}, {"P229"}},
    // NightHaven
    {"Wamai", {"AUG A2", "MP5K"}, {"Keratos .357", "P12"}},
    // Unaffiliated
    {"Oryx", {"T-5 SMG", "SPAS-12"}, {"Bailiff 410", "USP40"}},
    // Inkaba Task Force
    {"Melusi", {"MP5", "Super 90"}, {"RG15"}},
    // NightHaven
    {"Aruni", {"P10 Roni", "Mk 14 EBR"}, {"PRB92"}},
    // Star-Net Aviation
    {"Thunderbird", {"Spear .308", "SPAS-15"}, {"Bearing 9", "Q-929"}},
    // Emergency Response Unit
    {"Thorn", {"UZK50GI", "M870"}, {"1911 Tacops", "C71 Auto"}},
    // Unaffiliated
    {"Azami", {"9x19VSN", "ACS12"}, {"D-50"}},
    // Afeau
    {"Solis", {"P90", "ITA12L"}, {"SMG-11"}},
    // Unaffiliated
    {"Fenrir", {"MP7", "SASG-12"}, {"Bailiff 410", "5.7 USG"}},
    // DAE
    {"Tubarao", {"MPX", "AR-15.50"}, {"P226 MK 25"}},
};
struct WeaponData {
    std::string Operator;
    std::string Weapon;
    std::string Sight;
    std::string Grip;
    std::string Attachment;
};
WeaponData AttackerPData;
WeaponData AttackerSData;
WeaponData DefenderPData;
WeaponData DefenderSData;
void Weapon_Data_Save(const std::string& Operator, const std::string& Weapon, const std::string& Weapon_Sight, const std::string& Weapon_Grip, const std::string& Weapon_Attachment) {
    std::string FileName = "Weapon_Data.txt";
    std::ifstream inputFile(FileName);
    std::stringstream buffer;
    std::string line;
    bool found = false;
    while (std::getline(inputFile, line)) {
        if (line.find(Operator) != std::string::npos && line.find(Weapon) != std::string::npos) {
            found = true;
            line = "{ \"" + Operator + "\", \"" + Weapon + "\", { \"" + Weapon_Sight + "\", \"" + Weapon_Grip + "\", \"" + Weapon_Attachment + "\" } },";
        }
        buffer << line << std::endl;
    }
    if (!found) {
        buffer << "{ \"" << Operator << "\", \"" << Weapon << "\", { \"" << Weapon_Sight << "\", \"" << Weapon_Grip << "\", \"" << Weapon_Attachment << "\" } }," << std::endl;
    }
    inputFile.close();
    std::ofstream outputFile(FileName);
    if (outputFile.is_open()) {
        outputFile << buffer.str();
        outputFile.close();
        std::cout << "File updated successfully." << std::endl;
    }
    else {
        std::cerr << "Unable to open file for writing: " << FileName << std::endl;
    }
}
WeaponData Weapon_Data_Get(const std::string& Operator, const std::string& Weapon) {
    WeaponData data;
    std::string FileName = "Weapon_Data.txt";
    std::ifstream inputFile(FileName);
    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.find(Operator) != std::string::npos && line.find(Weapon) != std::string::npos) {
            size_t start = line.find("{") + 1;
            size_t end = line.find("}") - 1;
            std::string values = line.substr(start, end - start + 1);

            std::istringstream iss(values);
            std::string token;
            std::getline(iss, token, ',');
            data.Operator = token.substr(token.find("\"") + 1, token.find_last_of("\"") - token.find("\"") - 1);
            std::getline(iss, token, ',');
            data.Weapon = token.substr(token.find("\"") + 1, token.find_last_of("\"") - token.find("\"") - 1);
            std::getline(iss, token, ',');
            data.Sight = token.substr(token.find("\"") + 1, token.find_last_of("\"") - token.find("\"") - 1);
            std::getline(iss, token, ',');
            data.Grip = token.substr(token.find("\"") + 1, token.find_last_of("\"") - token.find("\"") - 1);
            std::getline(iss, token, ',');
            data.Attachment = token.substr(token.find("\"") + 1, token.find_last_of("\"") - token.find("\"") - 1);
            break;
        }
    }
    inputFile.close();
    return data;
}
bool Init = false;
bool Drawing::isActive() {
	return bDraw == true;
}
void Recoil(){
    while (true) {
        HANDLE SerialPort = CreateFile(L"COM6", GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if (SerialPort == INVALID_HANDLE_VALUE) {
            std::cerr << "Error opening serial port\n";
        }
        DCB serialParams = { 0 };
        serialParams.DCBlength = sizeof(serialParams);
        if (!GetCommState(SerialPort, &serialParams)) {
            std::cerr << "Error getting serial port state\n";
            CloseHandle(SerialPort);
        }
        serialParams.BaudRate = CBR_9600;
        serialParams.ByteSize = 8;
        serialParams.StopBits = ONESTOPBIT;
        serialParams.Parity = NOPARITY;
        if (!SetCommState(SerialPort, &serialParams)) {
            std::cerr << "Error setting serial port state\n";
            CloseHandle(SerialPort);
        }
        COMMTIMEOUTS timeouts = { 0 };
        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;
        if (!SetCommTimeouts(SerialPort, &timeouts)) {
            std::cerr << "Error setting serial port timeouts\n";
            CloseHandle(SerialPort);
        }
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8001) && (GetAsyncKeyState(VK_RBUTTON) & 0x8001)) {
            DWORD bytesWritten;
            std::string moveCom = std::to_string((int)(Recoil_X * Recoil_Sensitivity)) + ":" + std::to_string((int)(Recoil_Y * Recoil_Sensitivity));
            if (Recoil_Enabled) {
                WriteFile(SerialPort, moveCom.c_str(), moveCom.size(), &bytesWritten, 0);
            }
            Sleep(1);
        }
        CloseHandle(SerialPort);
    }
}
double CalculateCursorMovement(double Sens, double NewSens, double Pulldown, double Dpi) {
    double k = static_cast<double>(Sens * Pulldown) / Dpi;
    double cursorMovement = (k * Dpi) / NewSens;
    return cursorMovement;
}
void GetRecoil() {
    PulldownValue = CalculateCursorMovement(PreSetVertical, NewVertical, PullDownPixels, Dpi);
    if (SelectedSight == "None") {
        CurrentSight = 0;
    } if (SelectedSight == "Non Magnifying") {
        CurrentSight = 0;
    } if (SelectedSight == "1.5x Scope") {
        CurrentSight = round(X1_5 / (0.49 / (tan((0.49 * FOV) * M_PI / 180 / 2) / tan(FOV * M_PI / 180 / 2))) * PulldownValue * 0.49 * X1_5 - NewVertical);
    } if (SelectedSight == "2.0x Scope") {
        CurrentSight = round(X2_0 / (0.59 / (tan((0.59 * FOV) * M_PI / 180 / 2) / tan(FOV * M_PI / 180 / 2))) * PulldownValue * 0.59 * X2_0 - NewVertical);
    } if (SelectedSight == "2.5x Scope") {
        CurrentSight = round(X2_5 / (0.60 / (tan((0.90 * FOV) * M_PI / 180 / 2) / tan(FOV * M_PI / 180 / 2))) * PulldownValue * 0.60 * X2_5 - NewVertical);
    } if (SelectedSight == "3.0x Scope") {
        CurrentSight = round(X3_0 / (0.60 / (tan((0.90 * FOV) * M_PI / 180 / 2) / tan(FOV * M_PI / 180 / 2))) * PulldownValue * 0.60 * X3_0 - NewVertical);
    }
    if (SelectedGrip == "None") {
        CurrentGrip = 0;
    } if (SelectedGrip == "Vertical") {
        CurrentGrip = (-4 * (PreSetVertical / NewVertical));
    } if (SelectedGrip == "Angled") {
        CurrentGrip = (+0.1 * (PreSetVertical / NewVertical));
    }
    if (SelectedAttachment == "None") {
        CurrentAttachment = 0;
    } if (SelectedAttachment == "Flash Hider") {
        CurrentAttachment = (-2 * (PreSetVertical / NewVertical));
    } if (SelectedAttachment == "Compensator") {
        CurrentAttachment = 0;
    } if (SelectedAttachment == "Muzzle Brake") {
        CurrentAttachment = (-0.3 * (PreSetVertical / NewVertical));
    } if (SelectedAttachment == "Suppressor") {
        CurrentAttachment = 0;
    } if (SelectedAttachment == "Extended Barrel") {
        CurrentAttachment = (0.4 * (PreSetVertical / NewVertical));
    }
    double ChosenSight = CurrentSight;
    Sightpulldown = ChosenSight / 1000;
}
void LineWithText(const char  * Text) {
    ImGui::Text(Text);
    ImVec2 textPos = ImGui::GetItemRectMin();
    ImVec2 textSize = ImGui::CalcTextSize(Text);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float lineHeight = ImGui::GetTextLineHeight();
    ImVec2 lineStart(textPos.x, textPos.y + lineHeight);
    ImVec2 lineEnd(textPos.x + textSize.x, textPos.y + lineHeight);
    drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 255, 255, 255));
}
void Mover(std::string X, std::string Y, std::string Sight, std::string Grip, std::string Attachment) {
    PulldownValue = CalculateCursorMovement(PreSetVertical, NewVertical, std::stof(Y), Dpi);
    if (Sight == "None") {
        CurrentSight = 0;
    } if (Sight == "Non Magnifying") {
        CurrentSight = 0;
    } if (Sight == "1.5x Scope") {
        CurrentSight = round(X1_5 / (0.49 / (tan((0.49 * FOV) * M_PI / 180 / 2) / tan(FOV * M_PI / 180 / 2))) * PulldownValue * 0.49 * X1_5 - NewVertical);
    } if (Sight == "2.0x Scope") {
        CurrentSight = round(X2_0 / (0.59 / (tan((0.59 * FOV) * M_PI / 180 / 2) / tan(FOV * M_PI / 180 / 2))) * PulldownValue * 0.59 * X2_0 - NewVertical);
    } if (Sight == "2.5x Scope") {
        CurrentSight = round(X2_5 / (0.60 / (tan((0.90 * FOV) * M_PI / 180 / 2) / tan(FOV * M_PI / 180 / 2))) * PulldownValue * 0.60 * X2_5 - NewVertical);
    } if (Sight == "3.0x Scope") {
        CurrentSight = round(X3_0 / (0.60 / (tan((0.90 * FOV) * M_PI / 180 / 2) / tan(FOV * M_PI / 180 / 2))) * PulldownValue * 0.60 * X3_0 - NewVertical);
    }
    if (Grip == "None") {
        CurrentGrip = 0;
    } if (Grip == "Vertical") {
        CurrentGrip = (-4 * (PreSetVertical / NewVertical));
    } if (Grip == "Angled") {
        CurrentGrip = (+0.1 * (PreSetVertical / NewVertical));
    }
    if (Attachment == "None") {
        CurrentAttachment = 0;
    } if (Attachment == "Flash Hider") {
        CurrentAttachment = (-2 * (PreSetVertical / NewVertical));
    } if (Attachment == "Compensator") {
        CurrentAttachment = 0;
    } if (Attachment == "Muzzle Brake") {
        CurrentAttachment = (-0.3 * (PreSetVertical / NewVertical));
    } if (Attachment == "Suppressor") {
        CurrentAttachment = 0;
    } if (Attachment == "Extended Barrel") {
        CurrentAttachment = (0.4 * (PreSetVertical / NewVertical));
    }
    double ChosenSight = CurrentSight;
    Sightpulldown = ChosenSight / 1000;
    Recoil_X = std::stof(X);
    Recoil_Y = (PulldownValue + Sightpulldown + CurrentGrip + CurrentAttachment);
}
void GetRecoilValue(const std::string& Name, float& x, float& y) {
    for (const auto& weapon : Weapons) {
        if (weapon.Weapon_Name == Name) {
            x = std::stof(weapon.X[0]);
            y = std::stof(weapon.Y[0]);
            return;
        }
    }
    x = 0.0f;
    y = 0.0f;
}
void Watermark() {
    if (Watermark_Enabled) {
        if (Current_Side == "Attackers") {
            if (Current_Weapon_Slot == "Primary") {
                Watermark_Weapon = Attacker_Primary.c_str();
                Watermark_Sight = Attacker_Primary_Sight;
                Watermark_Attachment = Attacker_Primary_Attachment;
                Watermark_Grip = Attacker_Primary_Grip;
            }
            if (Current_Weapon_Slot == "Secondary") {
                Watermark_Weapon = Attacker_Secondary.c_str();
                Watermark_Sight = Attacker_Secondary_Sight;
                Watermark_Attachment = Attacker_Secondary_Attachment;
                Watermark_Grip = Attacker_Secondary_Grip;
            }
        }
        if (Current_Side == "Defenders") {
            if (Current_Weapon_Slot == "Primary") {
                Watermark_Weapon = Defender_Primary.c_str();
                Watermark_Sight = Defender_Primary_Sight;
                Watermark_Attachment = Defender_Primary_Attachment;
                Watermark_Grip = Defender_Primary_Grip;
            }
            if (Current_Weapon_Slot == "Secondary") {
                Watermark_Weapon = Defender_Secondary.c_str();
                Watermark_Sight = Defender_Secondary_Sight;
                Watermark_Attachment = Defender_Secondary_Attachment;
                Watermark_Grip = Defender_Secondary_Grip;
            }
        }
        Watermark_Text = std::string("Silent Solutions | Rainbow Six Siege\n") +Watermark_Weapon + std::string(" | ") + Watermark_Sight + std::string(" | ") + Watermark_Grip + std::string(" | ") + Watermark_Attachment;
        Cheat_Text = Cheat_Name + std::string(" | ") + Cheat_Game;
        ImVec2 Watermark_Text_Size = ImGui::CalcTextSize(Watermark_Text.c_str());
        Watermark_Text_Size.x += 12;
        Watermark_Text_Size.y += 15;
        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::SetNextWindowSize(Watermark_Text_Size, ImGuiCond_Once);
        ImGui::Begin("Watermark", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs);
        ImGui::Text(Watermark_Text.c_str());
        ImGui::SetWindowSize(Watermark_Text_Size);
        ImGui::End();
    }
}
void Drawing::Draw() {
    Watermark();
	if (isActive()) {
		if (!UI::IsWindowTargeted()) {
			std::vector<UI::WindowItem> WindowList;
			UI::GetAllWindow(&WindowList);
			if (WindowList.empty())
				return;
			ImGui::SetNextWindowSize({ 400, 400 }, ImGuiCond_Once);
			ImGui::SetNextWindowBgAlpha(1.0f);
            ImGui::Begin("Overlay Target Chooser", &bDraw, WindowFlags); {
				if (ImGui::BeginCombo("##combo", lpSelectedWindow.CurrentWindowTitle)) {
					for (const auto& item : WindowList) {
						const bool is_selected = (strcmp(lpSelectedWindow.CurrentWindowTitle, item.CurrentWindowTitle) == 0);
						if (ImGui::Selectable(item.CurrentWindowTitle, is_selected))
							lpSelectedWindow = item;
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::NewLine();
				if (ImGui::Button("Start Overlay")) {
					UI::SetTargetWindow(lpSelectedWindow.CurrentWindow);
				}
			}
			ImGui::End();
			return;
		}
        ImGui::SetNextWindowSize(vWindowSize, ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1.0f);
        ImGui::Begin(lpWindowName, &bDraw, WindowFlags); {
            if (ImGui::BeginTabBar("Tab_Bar")) {
                if (!Init) {
                    Init = true;
                    std::thread testThread(Recoil);
                    testThread.detach();
                }
                if (ImGui::BeginTabItem("Aimbot")) {
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Recoil")) {
                    ImGui::Checkbox("Enable Recoil", &Recoil_Enabled);
                    if (Current_Side == "Attackers") {
                        if (Current_Weapon_Slot == "Primary") {
                            float X_Value, Y_Value;
                            GetRecoilValue(Attacker_Primary, X_Value, Y_Value);
                            std::string Move_X_Value = std::to_string(X_Value + Randomisation_X_Value);
                            std::string Move_Y_Value = std::to_string(Y_Value + Randomisation_Y_Value);
                            Mover(Move_X_Value, Move_Y_Value, Attacker_Primary_Sight, Attacker_Primary_Grip, Attacker_Primary_Attachment);
                        } if (Current_Weapon_Slot == "Secondary") {
                            float X_Value, Y_Value;
                            GetRecoilValue(Attacker_Secondary, X_Value, Y_Value);
                            std::string Move_X_Value = std::to_string(X_Value + Randomisation_X_Value);
                            std::string Move_Y_Value = std::to_string(Y_Value + Randomisation_Y_Value);
                            Mover(Move_X_Value, Move_Y_Value, Attacker_Secondary_Sight, Attacker_Secondary_Grip, Attacker_Secondary_Attachment);
                        }
                    } if (Current_Side == "Defenders") {
                        if (Current_Weapon_Slot == "Primary") {
                            float X_Value, Y_Value;
                            GetRecoilValue(Defender_Primary, X_Value, Y_Value);
                            std::string Move_X_Value = std::to_string(X_Value + Randomisation_X_Value);
                            std::string Move_Y_Value = std::to_string(Y_Value + Randomisation_Y_Value);
                            Mover(Move_X_Value, Move_Y_Value, Defender_Primary_Sight, Defender_Primary_Grip, Defender_Primary_Attachment);
                        } if (Current_Weapon_Slot == "Secondary") {
                            float X_Value, Y_Value;
                            GetRecoilValue(Defender_Secondary, X_Value, Y_Value);
                            std::string Move_X_Value = std::to_string(X_Value + Randomisation_X_Value);
                            std::string Move_Y_Value = std::to_string(Y_Value + Randomisation_Y_Value);
                            Mover(Move_X_Value, Move_Y_Value, Defender_Secondary_Sight, Defender_Secondary_Grip, Defender_Secondary_Attachment);
                        }
                    }
                    if (ImGui::BeginTabBar("Operators")) {
                        if (ImGui::BeginTabItem("Attackers")) {
                            Current_Side = "Attackers";
                            if (ImGui::BeginCombo("Name", Attacker_Name.c_str())) {
                                for (const auto& Attacker : Attackers) {
                                    bool isSelected = (Defender_Name == Attacker.Operator_Name);
                                    if (ImGui::Selectable(Attacker.Operator_Name.c_str(), isSelected)) {
                                        Attacker_Name = Attacker.Operator_Name;
                                        Attacker_Primary = "";
                                        Attacker_Secondary = "";
                                        Attacker_Primary_Sight = "";
                                        Attacker_Primary_Grip = "";
                                        Attacker_Primary_Attachment = "";
                                        Attacker_Secondary_Sight = "";
                                        Attacker_Secondary_Grip = "";
                                        Attacker_Secondary_Attachment = "";
                                    }
                                    if (isSelected) {
                                        ImGui::SetItemDefaultFocus();
                                    }
                                }
                                ImGui::EndCombo();
                            }
                            if (!Attacker_Name.empty()) {
                                const auto& selectedAttacker = std::find_if(Attackers.begin(), Attackers.end(),
                                    [&](const Attacker& a) { return a.Operator_Name == Attacker_Name; });
                                if (selectedAttacker != Attackers.end()) {
                                    if (ImGui::BeginTabBar("Attacker_Weapons")) {
                                        if (ImGui::BeginTabItem("Primary")) {
                                            Current_Weapon_Slot = "Primary";
                                            if (ImGui::BeginCombo("Primary Weapon", Attacker_Primary.c_str())) {
                                                for (const auto& weapon : selectedAttacker->Primary_Weapons) {
                                                    bool isSelected = (Attacker_Primary == weapon);
                                                    if (ImGui::Selectable(weapon.c_str(), isSelected)) {
                                                        Attacker_Primary = weapon;
                                                        AttackerPData = Weapon_Data_Get(Attacker_Name, Attacker_Primary);
                                                        Attacker_Primary_Sight = AttackerPData.Sight.c_str();
                                                        Attacker_Primary_Grip = AttackerPData.Grip.c_str();
                                                        Attacker_Primary_Attachment = AttackerPData.Attachment.c_str();
                                                    }
                                                    if (isSelected)
                                                        ImGui::SetItemDefaultFocus();
                                                }
                                                ImGui::EndCombo();
                                            }
                                            if (ImGui::BeginCombo("Select Sight", Attacker_Primary_Sight)) {
                                                for (int i = 0; i < IM_ARRAYSIZE(SightList); i++) {
                                                    bool isSelected1 = (Attacker_Primary_Sight == SightList[i]);
                                                    if (ImGui::Selectable(SightList[i], isSelected1)) {
                                                        Attacker_Primary_Sight = SightList[i];
                                                        const char* Operatorap = Attacker_Name.c_str();
                                                        const char* Weaponap = Attacker_Primary.c_str();
                                                        const char* Scopeap = Attacker_Primary_Sight;
                                                        const char* Gripap = Attacker_Primary_Grip;
                                                        const char* Attachmentap = Attacker_Primary_Attachment;
                                                        Weapon_Data_Save(Operatorap, Weaponap, Scopeap, Gripap, Attachmentap);
                                                    }
                                                    if (isSelected1)
                                                        ImGui::SetItemDefaultFocus();
                                                }
                                                ImGui::EndCombo();
                                            }
                                            if (ImGui::BeginCombo("Select Grip", Attacker_Primary_Grip)) {
                                                for (int i = 0; i < IM_ARRAYSIZE(GripList); i++) {
                                                    bool isSelected2 = (Attacker_Primary_Grip == GripList[i]);
                                                    if (ImGui::Selectable(GripList[i], isSelected2)) {
                                                        Attacker_Primary_Grip = GripList[i];
                                                        const char* Operatorap = Attacker_Name.c_str();
                                                        const char* Weaponap = Attacker_Primary.c_str();
                                                        const char* Scopeap = Attacker_Primary_Sight;
                                                        const char* Gripap = Attacker_Primary_Grip;
                                                        const char* Attachmentap = Attacker_Primary_Attachment;
                                                        Weapon_Data_Save(Operatorap, Weaponap, Scopeap, Gripap, Attachmentap);
                                                    }
                                                    if (isSelected2)
                                                        ImGui::SetItemDefaultFocus();
                                                }
                                                ImGui::EndCombo();
                                            }
                                            if (ImGui::BeginCombo("Select Attachment", Attacker_Primary_Attachment)) {
                                                for (int i = 0; i < IM_ARRAYSIZE(AttachmentList); i++) {
                                                    bool isSelected3 = (Attacker_Primary_Attachment == AttachmentList[i]);
                                                    if (ImGui::Selectable(AttachmentList[i], isSelected3)) {
                                                        Attacker_Primary_Attachment = AttachmentList[i];
                                                        const char* Operatorap = Attacker_Name.c_str();
                                                        const char* Weaponap = Attacker_Primary.c_str();
                                                        const char* Scopeap = Attacker_Primary_Sight;
                                                        const char* Gripap = Attacker_Primary_Grip;
                                                        const char* Attachmentap = Attacker_Primary_Attachment;
                                                        Weapon_Data_Save(Operatorap, Weaponap, Scopeap, Gripap, Attachmentap);
                                                    }
                                                    if (isSelected3)
                                                        ImGui::SetItemDefaultFocus();
                                                }
                                                ImGui::EndCombo();
                                            }
                                            ImGui::EndTabItem();
                                        }
                                        if (ImGui::BeginTabItem("Secondary")) {
                                            Current_Weapon_Slot = "Secondary";
                                            if (ImGui::BeginCombo("Secondary Weapon", Attacker_Secondary.c_str())) {
                                                for (const auto& weapon : selectedAttacker->Secondary_Weapons) {
                                                    bool isSelected = (Attacker_Secondary == weapon);
                                                    if (ImGui::Selectable(weapon.c_str(), isSelected)) {
                                                        Attacker_Secondary = weapon;
                                                        AttackerSData = Weapon_Data_Get(Attacker_Name, Attacker_Secondary);
                                                        Attacker_Secondary_Sight = AttackerSData.Sight.c_str();
                                                        Attacker_Secondary_Grip = AttackerSData.Grip.c_str();
                                                        Attacker_Secondary_Attachment = AttackerSData.Attachment.c_str();
                                                        if (isSelected)
                                                            ImGui::SetItemDefaultFocus();
                                                    }
                                                }
                                                ImGui::EndCombo();
                                            }
                                            if (Attacker_Secondary == "SMG-11" || Attacker_Secondary == "Bearing 9" || Attacker_Secondary == "SMG-12" || Attacker_Secondary == "SPSMG9") {
                                                if (ImGui::BeginCombo("Select Sight", Attacker_Secondary_Sight)) {
                                                    for (int i = 0; i < IM_ARRAYSIZE(SightList); i++) {
                                                        bool isSelected1 = (Attacker_Secondary_Sight == SightList[i]);
                                                        if (ImGui::Selectable(SightList[i], isSelected1)) {
                                                            Attacker_Secondary_Sight = SightList[i];
                                                            const char* Operatoras = Attacker_Name.c_str();
                                                            const char* Weaponas = Attacker_Secondary.c_str();
                                                            const char* Scopeas = Attacker_Secondary_Sight;
                                                            const char* Gripas = Attacker_Secondary_Grip;
                                                            const char* Attachmentas = Attacker_Secondary_Attachment;
                                                            Weapon_Data_Save(Operatoras, Weaponas, Scopeas, Gripas, Attachmentas);
                                                        }
                                                        if (isSelected1) {
                                                            ImGui::SetItemDefaultFocus();
                                                        }
                                                    }

                                                    ImGui::EndCombo();
                                                }
                                                if (Attacker_Secondary == "SMG-11" || Attacker_Secondary == "SMG-12" || Attacker_Secondary == "SPSMG9") {
                                                    if (ImGui::BeginCombo("Select Grip", Attacker_Secondary_Grip)) {
                                                        for (int i = 0; i < IM_ARRAYSIZE(GripList); i++) {
                                                            bool isSelected2 = (Attacker_Secondary_Grip == GripList[i]);
                                                            if (ImGui::Selectable(GripList[i], isSelected2)) {
                                                                Attacker_Secondary_Grip = GripList[i];
                                                                const char* Operatoras = Attacker_Name.c_str();
                                                                const char* Weaponas = Attacker_Secondary.c_str();
                                                                const char* Scopeas = Attacker_Secondary_Sight;
                                                                const char* Gripas = Attacker_Secondary_Grip;
                                                                const char* Attachmentas = Attacker_Secondary_Attachment;
                                                                Weapon_Data_Save(Operatoras, Weaponas, Scopeas, Gripas, Attachmentas);
                                                            }
                                                            if (isSelected2) {
                                                                ImGui::SetItemDefaultFocus();
                                                            }
                                                        }
                                                        ImGui::EndCombo();
                                                    }
                                                }
                                                if (ImGui::BeginCombo("Select Attachment", Attacker_Secondary_Attachment)) {
                                                    for (int i = 0; i < IM_ARRAYSIZE(AttachmentList); i++) {
                                                        bool isSelected3 = (Attacker_Secondary_Attachment == AttachmentList[i]);
                                                        if (ImGui::Selectable(AttachmentList[i], isSelected3)) {
                                                            Attacker_Secondary_Attachment = AttachmentList[i];
                                                            const char* Operatoras = Attacker_Name.c_str();
                                                            const char* Weaponas = Attacker_Secondary.c_str();
                                                            const char* Scopeas = Attacker_Secondary_Sight;
                                                            const char* Gripas = Attacker_Secondary_Grip;
                                                            const char* Attachmentas = Attacker_Secondary_Attachment;
                                                            Weapon_Data_Save(Operatoras, Weaponas, Scopeas, Gripas, Attachmentas);
                                                        }
                                                        if (isSelected3) {
                                                            ImGui::SetItemDefaultFocus();
                                                        }
                                                    }
                                                    ImGui::EndCombo();
                                                }
                                            }
                                            else if (Attacker_Secondary == "Super Shorty" || Attacker_Secondary == "ITA12S" || Attacker_Secondary == "Bailiff 410" || Attacker_Secondary == "5.7 USG Special" || Attacker_Secondary == "GONNE-6" || Attacker_Secondary == ".44 Mag Semi-Auto") {
                                                ImGui::Text("Doesnt have any attachments.");
                                            }
                                            else {
                                                Attacker_Secondary_Grip = "None";
                                                Attacker_Secondary_Sight = "None";
                                                if (ImGui::BeginCombo("Select Attachment", Attacker_Secondary_Attachment)) {
                                                    for (int i = 0; i < IM_ARRAYSIZE(AttachmentList); i++) {
                                                        bool isSelected3 = (Attacker_Secondary_Attachment == AttachmentList[i]);
                                                        if (ImGui::Selectable(AttachmentList[i], isSelected3)) {
                                                            Attacker_Secondary_Attachment = AttachmentList[i];
                                                            const char* Operatoras = Attacker_Name.c_str();
                                                            const char* Weaponas = Attacker_Secondary.c_str();
                                                            const char* Scopeas = Attacker_Secondary_Sight;
                                                            const char* Gripas = Attacker_Secondary_Grip;
                                                            const char* Attachmentas = Attacker_Secondary_Attachment;
                                                            Weapon_Data_Save(Operatoras, Weaponas, Scopeas, Gripas, Attachmentas);
                                                        }
                                                        if (isSelected3) {
                                                            ImGui::SetItemDefaultFocus();
                                                        }
                                                    }
                                                    ImGui::EndCombo();
                                                }
                                            }
                                            ImGui::EndTabItem();
                                        }
                                    }
                                    ImGui::EndTabBar();
                                }
                            }
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Defenders")) {
                            Current_Side = "Defenders";
                            if (ImGui::BeginCombo("Name", Defender_Name.c_str())) {
                                for (const auto& Defender : Defenders) {
                                    bool isSelected = (Defender_Name == Defender.Operator_Name);
                                    if (ImGui::Selectable(Defender.Operator_Name.c_str(), isSelected)) {
                                        Defender_Name = Defender.Operator_Name;
                                        Defender_Primary = "";
                                        Defender_Secondary = "";
                                        Defender_Primary_Sight = "";
                                        Defender_Primary_Grip = "";
                                        Defender_Primary_Attachment = "";
                                        Defender_Secondary_Sight = "";
                                        Defender_Secondary_Grip = "";
                                        Defender_Secondary_Attachment = "";
                                    }
                                    if (isSelected) {
                                        ImGui::SetItemDefaultFocus();
                                    }
                                }
                                ImGui::EndCombo();
                            }
                            if (!Defender_Name.empty()) {
                                const auto& selectedDefender = std::find_if(Defenders.begin(), Defenders.end(),
                                    [&](const Defender& d) { return d.Operator_Name == Defender_Name; });
                                if (selectedDefender != Defenders.end()) {
                                    if (ImGui::BeginTabBar("Defenders_Weapons")) {
                                        if (ImGui::BeginTabItem("Primary")) {
                                            Current_Weapon_Slot = "Primary";
                                            if (ImGui::BeginCombo("Primary Weapon", Defender_Primary.c_str())) {
                                                for (const auto& weapon : selectedDefender->Primary_Weapons) {
                                                    bool isSelected = (Defender_Primary == weapon);
                                                    if (ImGui::Selectable(weapon.c_str(), isSelected)) {
                                                        Defender_Primary = weapon;
                                                        DefenderPData = Weapon_Data_Get(Defender_Name, Defender_Primary);
                                                        Defender_Primary_Sight = DefenderPData.Sight.c_str();
                                                        Defender_Primary_Grip = DefenderPData.Grip.c_str();
                                                        Defender_Primary_Attachment = DefenderPData.Attachment.c_str();
                                                        if (isSelected)
                                                            ImGui::SetItemDefaultFocus();
                                                    }
                                                }
                                                ImGui::EndCombo();
                                            }
                                            if (ImGui::BeginCombo("Select Sight", Defender_Primary_Sight)) {
                                                for (int i = 0; i < IM_ARRAYSIZE(SightList); i++) {
                                                    bool isSelected1 = (Defender_Primary_Sight == SightList[i]);
                                                    if (ImGui::Selectable(SightList[i], isSelected1))
                                                        Defender_Primary_Sight = SightList[i];
                                                        const char* Operator = Defender_Name.c_str();
                                                        const char* Weapon = Defender_Primary.c_str();
                                                        const char* Scope = Defender_Primary_Sight;
                                                        const char* Grip = Defender_Primary_Grip;
                                                        const char* Attachment = Defender_Primary_Attachment;
                                                        Weapon_Data_Save(Operator, Weapon, Scope, Grip, Attachment);
                                                    if (isSelected1)
                                                        ImGui::SetItemDefaultFocus();
                                                }
                                                ImGui::EndCombo();
                                            }
                                            if (ImGui::BeginCombo("Select Grip", Defender_Primary_Grip)) {
                                                for (int i = 0; i < IM_ARRAYSIZE(GripList); i++) {
                                                    bool isSelected2 = (Defender_Primary_Grip == GripList[i]);
                                                    if (ImGui::Selectable(GripList[i], isSelected2))
                                                        Defender_Primary_Grip = GripList[i];
                                                        const char* Operator = Defender_Name.c_str();
                                                        const char* Weapon = Defender_Primary.c_str();
                                                        const char* Scope = Defender_Primary_Sight;
                                                        const char* Grip = Defender_Primary_Grip;
                                                        const char* Attachment = Defender_Primary_Attachment;
                                                        Weapon_Data_Save(Operator, Weapon, Scope, Grip, Attachment);
                                                    if (isSelected2)
                                                        ImGui::SetItemDefaultFocus();
                                                }
                                                ImGui::EndCombo();
                                            }
                                            if (ImGui::BeginCombo("Select Attachment", Defender_Primary_Attachment)) {
                                                for (int i = 0; i < IM_ARRAYSIZE(AttachmentList); i++) {
                                                    bool isSelected3 = (Defender_Primary_Attachment == AttachmentList[i]);
                                                    if (ImGui::Selectable(AttachmentList[i], isSelected3))
                                                        Defender_Primary_Attachment = AttachmentList[i];
                                                        const char* Operator = Defender_Name.c_str();
                                                        const char* Weapon = Defender_Primary.c_str();
                                                        const char* Scope = Defender_Primary_Sight;
                                                        const char* Grip = Defender_Primary_Grip;
                                                        const char* Attachment = Defender_Primary_Attachment;
                                                        Weapon_Data_Save(Operator, Weapon, Scope, Grip, Attachment);
                                                    if (isSelected3)
                                                        ImGui::SetItemDefaultFocus();
                                                }
                                                ImGui::EndCombo();
                                            }
                                            ImGui::EndTabItem();
                                        }
                                        if (ImGui::BeginTabItem("Secondary")) {
                                            Current_Weapon_Slot = "Secondary";
                                            if (ImGui::BeginCombo("Secondary Weapon", Defender_Secondary.c_str())) {
                                                for (const auto& weapon : selectedDefender->Secondary_Weapons) {
                                                    bool isSelected = (Defender_Secondary == weapon);
                                                    if (ImGui::Selectable(weapon.c_str(), isSelected)) {
                                                        Defender_Secondary = weapon;
                                                        DefenderSData = Weapon_Data_Get(Defender_Name, Defender_Secondary);
                                                        Defender_Secondary_Sight = DefenderSData.Sight.c_str();
                                                        Defender_Secondary_Grip = DefenderSData.Grip.c_str();
                                                        Defender_Secondary_Attachment = DefenderSData.Attachment.c_str();
                                                        if (isSelected)
                                                            ImGui::SetItemDefaultFocus();
                                                    }
                                                }
                                                ImGui::EndCombo();
                                            }
                                            if (Defender_Secondary == "SMG-11" || Defender_Secondary == "Bearing 9" || Defender_Secondary == "SMG-12" || Defender_Secondary == "SPSMG9") {
                                                if (ImGui::BeginCombo("Select Sight", Defender_Secondary_Sight)) {
                                                    for (int i = 0; i < IM_ARRAYSIZE(SightList); i++) {
                                                        bool isSelected1 = (Defender_Secondary_Sight == SightList[i]);
                                                        if (ImGui::Selectable(SightList[i], isSelected1)) {
                                                            Defender_Secondary_Sight = SightList[i];
                                                            const char* Operator = Defender_Name.c_str();
                                                            const char* Weapon = Defender_Secondary.c_str();
                                                            const char* Scope = Defender_Secondary_Sight;
                                                            const char* Grip = Defender_Secondary_Grip;
                                                            const char* Attachment = Defender_Secondary_Attachment;
                                                            Weapon_Data_Save(Operator, Weapon, Scope, Grip, Attachment);
                                                        }
                                                        if (isSelected1) {
                                                            ImGui::SetItemDefaultFocus();
                                                        }
                                                    }
                                                    ImGui::EndCombo();
                                                }
                                                if (Defender_Secondary == "SMG-11" || Defender_Secondary == "SMG-12" || Defender_Secondary == "SPSMG9") {
                                                    if (ImGui::BeginCombo("Select Grip", Defender_Secondary_Grip)) {
                                                        for (int i = 0; i < IM_ARRAYSIZE(GripList); i++) {
                                                            bool isSelected2 = (Defender_Secondary_Grip == GripList[i]);
                                                            if (ImGui::Selectable(GripList[i], isSelected2)) {
                                                                Defender_Secondary_Grip = GripList[i];
                                                                const char* Operator = Defender_Name.c_str();
                                                                const char* Weapon = Defender_Secondary.c_str();
                                                                const char* Scope = Defender_Secondary_Sight;
                                                                const char* Grip = Defender_Secondary_Grip;
                                                                const char* Attachment = Defender_Secondary_Attachment;
                                                                Weapon_Data_Save(Operator, Weapon, Scope, Grip, Attachment);
                                                            }
                                                            if (isSelected2) {
                                                                ImGui::SetItemDefaultFocus();
                                                            }
                                                        }
                                                        ImGui::EndCombo();
                                                    }
                                                }
                                                if (ImGui::BeginCombo("Select Attachment", Defender_Secondary_Attachment)) {
                                                    for (int i = 0; i < IM_ARRAYSIZE(AttachmentList); i++) {
                                                        bool isSelected3 = (Defender_Secondary_Attachment == AttachmentList[i]);
                                                        if (ImGui::Selectable(AttachmentList[i], isSelected3)) {
                                                            Defender_Secondary_Attachment = AttachmentList[i];
                                                            const char* Operator = Defender_Name.c_str();
                                                            const char* Weapon = Defender_Secondary.c_str();
                                                            const char* Scope = Defender_Secondary_Sight;
                                                            const char* Grip = Defender_Secondary_Grip;
                                                            const char* Attachment = Defender_Secondary_Attachment;
                                                            Weapon_Data_Save(Operator, Weapon, Scope, Grip, Attachment);
                                                        }
                                                        if (isSelected3) {
                                                            ImGui::SetItemDefaultFocus();
                                                        }
                                                    }
                                                    ImGui::EndCombo();
                                                }
                                            }
                                            else if (Defender_Secondary == "Super Shorty" || Defender_Secondary == "Bailiff 410" || Defender_Secondary == "LUISON") {
                                                ImGui::Text("Doesnt have any attachments.");
                                            }
                                            else {
                                                Defender_Secondary_Grip = "None";
                                                Defender_Secondary_Sight = "None";
                                                if (ImGui::BeginCombo("Select Attachment", Defender_Secondary_Attachment)) {
                                                    for (int i = 0; i < IM_ARRAYSIZE(AttachmentList); i++) {
                                                        bool isSelected3 = (Defender_Secondary_Attachment == AttachmentList[i]);
                                                        if (ImGui::Selectable(AttachmentList[i], isSelected3)) {
                                                            Defender_Secondary_Attachment = AttachmentList[i];
                                                            const char* Operator = Defender_Name.c_str();
                                                            const char* Weapon = Defender_Secondary.c_str();
                                                            const char* Scope = Defender_Secondary_Sight;
                                                            const char* Grip = Defender_Secondary_Grip;
                                                            const char* Attachment = Defender_Secondary_Attachment;
                                                            Weapon_Data_Save(Operator, Weapon, Scope, Grip, Attachment);
                                                        }
                                                        if (isSelected3) {
                                                            ImGui::SetItemDefaultFocus();
                                                        }
                                                    }
                                                    ImGui::EndCombo();
                                                }
                                            }
                                            ImGui::EndTabItem();
                                        }
                                        ImGui::EndTabBar();
                                    }
                                }
                            }
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Settings")) {
                    if (ImGui::BeginTabBar("Settings_Bar")) {
                        if (ImGui::BeginTabItem("Recoil")) {
                            LineWithText("Randomisation");
                            ImGui::Text("Adds random value to weapons when selected\nnot continuously changing");
                            ImGui::Checkbox("Enable", &Randomisation_Enable);
                            ImGui::SliderFloat("X Max Value", &Randomisation_X_Slider, 0.0f, 1.0f);
                            ImGui::SliderFloat("Y Max Value", &Randomisation_Y_Slider, 0.0f, 1.0f);
                            if (Randomisation_Enable) {
                                float Max_X = Randomisation_X_Slider;
                                float Max_Y = Randomisation_Y_Slider;
                                std::random_device rd;
                                std::mt19937 gen(rd());
                                std::uniform_real_distribution<float> distrib_x(0.0f, Max_X);
                                std::uniform_real_distribution<float> distrib_y(0.0f, Max_Y);
                                Randomisation_X_Value = distrib_x(gen);
                                Randomisation_Y_Value = distrib_y(gen);
                            }
                            else {
                                Randomisation_X_Value = 0;
                                Randomisation_Y_Value = 0;
                            }
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("In-Game")) {
                            ImGui::InputInt("FOV", &FOV);
                            LineWithText("Mouse Sensitivty");
                            ImGui::InputInt("Horizontal", &NewHorizontal);
                            ImGui::InputInt("Vertical", &NewVertical);
                            LineWithText("Sight Magnification Sensitivty");
                            ImGui::InputInt("1.5x", &X1_5);
                            ImGui::InputInt("2.0x", &X2_0);
                            ImGui::InputInt("2.5x", &X2_5);
                            ImGui::InputInt("3.0x", &X3_0);
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Gui")) {
                            ImGui::Checkbox("Watermark", &Watermark_Enabled);
                            if (ImGui::Button("Close Process")) {
                                UI::CloseProcess();
                            }
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Developer Options")) {
                    ImGui::Text("Side: %s", Current_Side);
                    ImGui::Text("Slot: %s", Current_Weapon_Slot);
                    ImGui::SliderFloat("X", &Recoil_X, -20.0f, 20.0f);
                    ImGui::SliderFloat("Y", &Recoil_Y, -20.0f, 80.0f);
                    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
                    ImGui::InputFloat("PullDown Value", &PullDownPixels);
                    if (ImGui::BeginCombo("Select Sight", SelectedSight)) {
                        for (int i = 0; i < IM_ARRAYSIZE(SightList); i++) {
                            bool isSelected1 = (SelectedSight == SightList[i]);
                            if (ImGui::Selectable(SightList[i], isSelected1))
                                SelectedSight = SightList[i];
                            if (isSelected1)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    if (ImGui::BeginCombo("Select Grip", SelectedGrip)) {
                        for (int i = 0; i < IM_ARRAYSIZE(GripList); i++) {
                            bool isSelected2 = (SelectedGrip == GripList[i]);
                            if (ImGui::Selectable(GripList[i], isSelected2))
                                SelectedGrip = GripList[i];
                            if (isSelected2)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    if (ImGui::BeginCombo("Select Attachment", SelectedAttachment)) {
                        for (int i = 0; i < IM_ARRAYSIZE(AttachmentList); i++) {
                            bool isSelected3 = (SelectedAttachment == AttachmentList[i]);
                            if (ImGui::Selectable(AttachmentList[i], isSelected3))
                                SelectedAttachment = AttachmentList[i];
                            if (isSelected3)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    if (ImGui::Button("Get New Recoil")) {
                        GetRecoil();
                    }
                    ImGui::Text("Sight: %s", SelectedSight);
                    ImGui::Text("Grip: %s", SelectedGrip);
                    ImGui::Text("Attachment: %s", SelectedAttachment);
                    ImGui::Text("%.1f Pulldown", PulldownValue);
                    ImGui::Text("%.1f SightPullDown", Sightpulldown);
                    ImGui::Text("%.1f Pulldown + SightPullDown", PulldownValue + Sightpulldown);
                    ImGui::Text("%.1f Grip", CurrentGrip);
                    ImGui::Text("%.1f Attachment", CurrentAttachment);
                    ImGui::Text("%.1f All Including Attachments", PulldownValue + Sightpulldown + CurrentGrip + CurrentAttachment);
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
		}
		ImGui::End();
	}
    if (GetAsyncKeyState(VK_F4) & 1) {
        bDraw = !bDraw;
    }
}