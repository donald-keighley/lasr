# Note! Ver 3.0 specifications are preliminary! Do not assume they will not change.
# This is strictly an example to illustrate the proposed changes!
~Version
VERS. 3.0 : CWLS LOG ASCII STANDARD - VERSION 3.0
 WRAP. NO : ONE LINE PER DEPTH STEP
DLM . COMMA : DELIMITING CHARACTER (SPACE TAB OR COMMA)
~Well
#MNEM.UNIT DATA DESCRIPTION
#----- ----- ---------- -------------------------
 STRT .M 1660.1250 : First Index Value
 STOP .M 1660.8750 : Last Index Value
 STEP .M 0.1250 : STEP
NULL . -999.25 : NULL VALUE
COMP . ANY OIL COMPANY INC. : COMPANY
WELL . ANY ET AL 01-02-03-04 : WELL
FLD . WILDCAT : FIELD
LOC . 1-2-3-4W5M : LOCATION
SRVC . ANY LOGGING COMPANY INC. : SERVICE COMPANY
DATE . 13/12/1986 : Service DATE {DD/MM/YYYY}
CTRY . CA : COUNTRY
PROV . ALBERTA : PROVINCE
UWI . 100010200304W500 : UNIQUE WELL ID
LIC . 0123456 : LICENSE NUMBER
LATI . 45.37° 12' 58" : X LOCATION
LONG . 13.22° 30' 09" : Y LOCATION
GDAT . NAD83 : Geodetic Datum
~Parameter
#MNEM.UNIT VALUE DESCRIPTION
#-------------- ---------------- ---------------------------
#Required Parameters
PDAT . GL : Permanent Data
 APD .M 4.2 : Above Permanent Data
DREF . KB : Depth Reference (KB,DF,CB)
 EREF .M 234.5 : Elevation of Depth Reference
RUN . 1 : Run Number
#Defined Run Related parameters
RUNS . 2 : # of Runs for this well. {I}
RUN[1]. 2 : Number of the indexed RUN {I}
RUN[2]. 3 : Number of the indexed RUN {I}
 RUN_Depth[1].M 0.0,1500.0 : Run 1 Depth Interval {F}
 RUN_Depth[2].M 1500.0,2513.0 : Run 2 Depth Interval {F}
#Parameters that are zoned.
 NMAT_Depth[1].M 523,1500 : Neutron Matrix Depth interval {F}
 NMAT_Depth[2].M 1500,2500 : Neutron Matrix Depth interval {F}
 DMAT_Depth[1].M 523,1510 : Density Matrix Depth interval {F}
 DMAT_Depth[2].M 1510,2510 : Density Matrix Depth interval {F}
#Parameters that have different values over different intervals
MATR[1]. SAND : Neutron Porosity Matrix | NMAT_Depth[1]
MATR[2]. LIME : Neutron Porosity Matrix | NMAT_Depth[2]
MDEN[1].KG/M3 2650 : Neutron Porosity Matrix | DMAT_Depth[1]
MDEN[2].KG/M3 2710 : Neutron Porosity Matrix | DMAT_Depth[2]
#Defined First/Last channel readings parameters
 FR_LR[1].M 500,100 : | DT
 FR_LR[2].M 523,100 : | DPHI
 FR_LR[3].M 520,100 : | NPHI
 FR_LR[4].M 500,100 : | YME
 FR_LR[5].M : | CDES
 FR_LR[6].M 510,100 : | NMR[1]
 FR_LR[7].M 510,100 : | NMR[2]
 FR_LR[8].M 510,100 : | NMR[3]
 FR_LR[9].M 510,100 : | NMR[4]
 FR_LR[10].M 510,100 : | NMR[5]
#Required parameters for AEUB compliance (but not LAS compliance
 TDL .M : Total Depth Logger | RUN_Depth[1]
 TDD .M : Total Depth Driller | RUN_Depth[1]
 CSGL .M : Casing Bottom Logger | RUN_Depth[1]
 CSGD .M : Casing Bottom Driller | RUN_Depth[1]
 CSGS .MM : Casing Size | RUN_Depth[1]
 CSGW .KG/M : Casing Weight | RUN_Depth[1]
 BS .MM : Bit Size | RUN_Depth[1]
MUD . : Mud type | RUN_Depth[1]
MUDS . : Mud Source | RUN_Depth[1]
 MUDD .KG/M3 : Mud Density | RUN_Depth[1]
 MUDV .S : Mud Viscosity (Funnel) | RUN_Depth[1]
 FL .CC : Fluid Loss | RUN_Depth[1]
PH . : PH | RUN_Depth[1]
RM .OHMM : Resistivity of Mud | RUN_Depth[1]
RMT .DEGC : Temperature of Mud | RUN_Depth[1]
RMF .OHMM : Rest. of Mud Filtrate | RUN_Depth[1]
RMFT .DEGC : Temp. of Mud Filtrate | RUN_Depth[1]
RMC .OHMM : Rest. of Mud Cake | RUN_Depth[1]
RMCT .DEGC : Temp. of Mud Cake | RUN_Depth[1]
TMAX .DEGC : Max. Recorded Temp. | RUN_Depth[1]
TIMC . : Date/Time Circulation Stopped | RUN_Depth[1]
TIML . : Date/Time Logger Tagged Bottom | RUN_Depth[1]
UNIT . : Logging Unit Number | RUN_Depth[1]
BASE . : Home Base of Logging Unit | RUN_Depth[1]
ENG . : Recording Engineer | RUN_Depth[1]
WIT . : Witnessed By | RUN_Depth[1]
#Next Run parameters
 TDL .M : Total Depth Logger | RUN_Depth[2]
 TDD .M : Total Depth Driller | RUN_Depth[2]
 CSGL .M : Casing Bottom Logger | RUN_Depth[2]
 CSGD .M : Casing Bottom Driller | RUN_Depth[2]
 CSGS .MM : Casing Size | RUN_Depth[2]
 CSGW .KG/M : Casing Weight | RUN_Depth[2]
 BS .MM : Bit Size | RUN_Depth[2]
MUD . : Mud type | RUN_Depth[2]
MUDS . : Mud Source | RUN_Depth[2]
 MUDD .KG/M3 : Mud Density | RUN_Depth[2]
 MUDV .S : Mud Viscosity (Funnel) | RUN_Depth[2]
 FL .CC : Fluid Loss | RUN_Depth[2]
PH . : PH | RUN_Depth[2]
RM .OHMM : Resistivity of Mud | RUN_Depth[2]
RMT .DEGC : Temperature of Mud | RUN_Depth[2]
RMF .OHMM : Rest. of Mud Filtrate | RUN_Depth[2]
RMFT .DEGC : Temp. of Mud Filtrate | RUN_Depth[2]
RMC .OHMM : Rest. of Mud Cake | RUN_Depth[2]
RMCT .DEGC : Temp. of Mud Cake | RUN_Depth[2]
TMAX .DEGC : Max. Recorded Temp. | RUN_Depth[2]
TIMC . : Date/Time Circulation Stopped | RUN_Depth[2]
TIML . : Date/Time Logger Tagged Bottom | RUN_Depth[2]
UNIT . : Logging Unit Number | RUN_Depth[2]
BASE . : Home Base of Logging Unit | RUN_Depth[2]
ENG . : Recording Engineer | RUN_Depth[2]
WIT . : Witnessed By | RUN_Depth[2]
~Curve
#MNEM.UNIT LOG CODES CURVE DESCRIPTION
#------------------ ------------ -------------------------
 DEPT .M : DEPTH {F}
 DT .US/M 123 456 789 : SONIC TRANSIT TIME {F}
 DPHI .V/V 123 456 789 : DENSITY POROSITY {F} | MDEN[1],MDEN[2]
 NPHI .V/V 123 456 789 : NEUTRON POROSITY {F} | MATR[1],MATR[2]
 YME .PA 123 456 789 : YOUNGS MODULES {E0.00E+00}
CDES . 123 456 789 : CORE DESCRIPTION {S}
# A 3D array channel begins here. It has 5 elements, and the amplitude is in
# millivolts
NMR[1] .mv 123 456 789 : NMR Echo Array {AF;0ms}
NMR[2] .mv 123 456 789 : NMR Echo Array {AF;5ms}
NMR[3] .mv 123 456 789 : NMR Echo Array {AF;10ms}
NMR[4] .mv 123 456 789 : NMR Echo Array {AF;15ms}
NMR[5] .mv 123 456 789 : NMR Echo Array {AF;20ms}
# The next 5 sets of 3 sections each are the newly defined sections.
# The ~ names are # now defined and must not be used for other sections.
# Drilling data section
~Drilling_Parameter
RIG . BIG RIG : Drilling Rig name
CONTR . DLR DRILLING : Contractor
~Drilling_Definition
 DEPT .ft : Depth {F}
 DIST .ft : Cumulative increment of drilling. {F}
HRS .hour : Hours of drilling {F}
 ROP .ft/hr : Rate of Penetration {F}
 WOB .klb : Weight on bit {F}
 RPM .RPM : Rotations per minute {F}
TQ .AMPS : Torque on bit in amps {F}
 PUMP .psi : Mud pump pressure {F}
 TSPM .SPM : Total strokes per minute {F}
 GPM .gal/min : Gallons per minute {F}
 ECD .ppg : Effective circulation density {F}
TBR . : Total barrels returned {F}
~Drilling_Data | Drilling_definition
322.02,1.02,0.0,24.0,3.0,59.0,111.0,1199.0,179.0, 879,8.73,39
323.05,2.05,0.1,37.5,2.0,69.0,118.0,1182.0,175.0, 861,8.73,202
~Core_Parameter[1]
 C_SRS . : Core Source {S}
 C_TY . : Core Type {S}
 C_DT . : Recovery Date (Date Core Cut) {DD/MM/YYYY}
 C_TP .M : Core Top Depth {F}
 C_BS .M : Core Base Depth {F}
 C_RC .M : Recovered Amount (Length) {F}
 C_FM . : Primary Formation Cored {S}
 C_DI .mm : Core Diameter {F}
 C_AC . : Analyzing Company {S}
 C_AD . : Analysis Date {DD/MM/YYYY}
~Core_Definition[1]
 CORT .M : Core top depth {F}
 CORB .M : Core Bottom Depth {F}
 PERM .md : Core permeability {F}
CPOR .% : Core porosity {F}
OIL .% : Core Oil saturation {F}
WTR .% : Core water saturation {F}
Oilvol.% : Core oil volume {F}
GAS .% : Core gas saturation {F}
WTR .% : Core water volume {F}
CDES . : Core description {S}
~Core_Data[1] | Core_Definition[1]
13178.00, 13179.00, 5.00, 17.70, 41.20, 40.10, 7.30, 3.30, 67.00,
13180.00, 13181.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00,
13182.00, 13183.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00,
13184.00, 13185.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00,
13186.00, 13187.00, 0.10, 13.30, 23.00, 87.20, 3.00, 1.40, 71.00,
13188.00, 13189.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00,
13190.00, 13191.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00,
13192.00, 13193.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00,
13194.00, 13195.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00,
13196.00, 13197.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00,
13458.00, 13459.00, 460.00, 29.60, 28.40, 47.90, 8.40, 7.00, 40.00,
13460.00, 13461.00, 430.00, 28.70, 62.00, 20.20, 17.80, 5.10, 39.00, VfgrU SliShy
13462.00, 13463.00, 180.00, 263.00, 59.70, 183.00, 15.70, 5.80, 46.00, VfgrU SliShy-Shy
13464.00, 13465.00, 150.00, 26.20, 48.90, 33.60, 12.80, 4.60, 48.00, VfgrU SliShy-Shy
13466.00, 13467.00, 43.00, 15.40, 16.40, 40.00, 25.00, 6.70, 63.00, VFgrL VShy
13468.00, 13469.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00,
13470.00, 13471.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00,
13472.00, 13473.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, Sdy WellCem
13474.00, 13475.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, Sdy WellCem
13476.00, 13477.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, -999.00, Sdy WellCem
~Core_Parameter[2]
 C_SRS . : Core Source {S}
 C_TY . : Core Type {S}
 C_DT . : Recovery Date (Date Core Cut) {DD/MM/YYYY}
 C_TP .M : Core Top Depth {F}
 C_BS .M : Core Base Depth {F}
 C_RC .M : Recovered Amount (Length) {F}
 C_FM . : Primary Formation Cored {S}
 C_DI .mm : Core Diameter {F}
 C_AC . : Analyzing Company {S}
 C_AD . : Analysis Date {DD/MM/YYYY}
~Core_Definition[2]
 CORT .M : Core top depth {F}
 CORB .M : Core Bottom Depth {F}
 PERM .md : Core permeability {F}
 CPOR .PU : Core porosity {F}
~Core_Data[2] | Core_definition[2]
13178.00, 13178.00, 5.00, 17.70
13180.00, 13180.00, -999.00, -999.00
13182.00, 13182.00, -999.00, -999.00
13184.00, 13184.00, -999.00, -999.00
~Inclinometry_Parameter
 I_DT . : SURVEY_DATE {DD/MM/YYYY}
 I_CO . : Recording Company {S}
 I_RF .M : Depth Datum Elevation (from MSL) {F}
 I_AT . : Azimuth North Type (e.g. Grid/ True) {S}
 I_DC .DEG : Magnetic Declination (if I_AT not magnetic) {F}
 I_KO .M : Kickoff Depth (M.D. of kickoff point) {F}
 I_GD . : Geodetic datum {S}
 I_ONS . : N/S Offset of well ref point to top hole {F}
 I_OEW .Unit : E/W Offset of well ref point to top hole {F}
 I_CP . : COMPUTE_METHOD (e.g. Radius of Curvature) {S}
 I_CS . : COORDINATE_SYSTEM_NAME eg UTM18N {S}
 TIEMD .M 0 : Tie Point Measured depth
 TIETVD .M 0 : Tie Point True Vertical depth
 TIEDEV .M 0 : Tie Point Deviation
 TIEAZM .M 0 : Tie Point Azimuth
 TIENSDR.M 0 : Tie Point North South drift
 TIEEWDR.M 0 : Tie Point East West drift
~Inclinometry_Definition
 MD .M : Measured Depth {F}
 TVD .M : True Vertical Depth {F}
 AZIM .DEG : Borehole Azimuth {F}
 DEVI .DEG : Borehole Deviation {F}
 RB .DEG : Relative Bearing {F}
~Inclinometry_Data | Inclinometry_definition
0.00,0.00,290.00,0.00,45.0
100.00,100.00,234.00,0.00,45.0
200.00,198.34,284.86,1.43,45.0
300.00,295.44,234.21,2.04,45.0
400.00,390.71,224.04,3.93,45.0
500.00,482.85,224.64,5.88,45.0
600.00,571.90,204.39,7.41,45.0
~Test_Parameter
TESTT. DST :Test Type {S}
~Test_Definition
TSTN . :TEST Number {I}
 TSTT .M :TEST Top Depth {F}
 TSTB .M :TEST Bottom Depth {F}
DDES . :TEST Recovery Description{S}
ISIP .KPAA :Initial Shut in pressure {F}
FSIP .KPAA :Final Shut in pressure {F}
RATE .BOPD :Production Rate {F}
BLOWD. :BLOW DESCRIPTION {S}
~Test_Data | Test_definition
1,1500,1505,50ft oil,13243,13350,10000,TSTM
2,2210,2235,Oil to surface,21451,21500,10000,Air
3,2575.0,2589.0,Packer Failure,0.0,0.0,0.0,TSTM
~Tops_Parameter
TOPS. Prognosis : Formation Source {S}
TOPDR. Subsea : Tops Depth Reference {S}
~Tops_Definition
TOPT.M : Formation Top Depth {F}
TOPB.M : Formation Base Depth {F}
TOPN. : Formation Top Name {S}
~Tops_Data | Tops_Definition
-1545.50,-1603.00,Viking
-1603.00,-1614.80,Colony
-1614.80,-1656.00,Basal Quartz
~Perforation_Parameter
PERFTYPE. 55 gr BIG HOLE : Charge Type {S}
~Perforation_Definition
PERFT.M : Perforation Top Depth {F}
PERFB.M : Perforation Bottom Depth {F}
 PERFD.SHOTS/M : Shot density per meter {F}
~Perforation_Data | Perforation_Definition
 545.50,550.60,12.0
 551.20,554.90,12.0
 575.00,595.00,12.0
~Ascii
 1660.125,123.450,0.110,0.370,1.45E+12,DOLOMITE W/VUGS,10.0,12.0,14.0,18.0,13.0
 1660.250,123.450,0.120,0.360,1.47E+12,LIMESTONE,12.0,15.0,21.0,35.0,25.0
 1660.375,123.450,0.130,0.350,2.85E+12,LOST INTERVAL,18.0,25.0,10.0,8.0,17.0
 1660.500,123.450,0.140,0.340,2.85E+12,LOST INTERVAL,18.0,25.0,10.0,8.0,17.0
 1660.625,123.450,0.150,0.330,2.85E+12,LOST INTERVAL,18.0,25.0,10.0,8.0,17.0
 1660.750,123.450,0.160,0.320,2.85E+12,"SANDSTONE, SHALE STREAKS",18.0,25.0,10.0,8.0,17.0
 1660.875,123.450,0.170,0.310,2.85E+12,LOST INTERVAL,18.0,25.0,10.0,8.0,17.0