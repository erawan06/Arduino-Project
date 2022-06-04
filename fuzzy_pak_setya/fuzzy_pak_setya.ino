#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a>b?a:b)

String FuzzyLogic(int temp, int humidity);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(FuzzyLogic(20, 30));
}

void loop() {
  // put your main code here, to run repeatedly:
}

String FuzzyLogic(int temp, int humidity) {
  //  Input Membership Function Temperature
  float imf_temp[5][3] = {
    {0.0f, 10.0f, 20.0f} ,  // Very Cold
    {10.0f, 20.0f, 30.0f}, // Cold
    {20.0f, 30.0f, 40.0f}, // Normal
    {30.0f, 40.0f, 50.0f}, // Warm
    {40.0f, 50.0f, 60.0f}  // Hot
  };

  //  Input Membership Function Humidity
  float imf_hum[5][3] = {
    {0.0f, 16.67f, 33.34f} ,// Too Dry
    {16.67f, 33.34f, 50.01f},  // Dry
    {33.34f, 50.01f, 66.68f},  // Ideal
    {50.01f, 66.68f, 83.35f},  // Humid
    {66.68f, 83.35f, 100.0f}  // Too Humid
  };

  // Output Membership Function Water Sprinkling
  float omf_water[5][5] = {
    {0.0f, 16.67f, 33.34f}, // No Water
    {16.67f, 33.34f, 50.01f}, // Less Water
    {33.34f, 50.01f, 66.68f}, // Medium Water
    {50.01f, 66.68f, 83.35f}, // More Water
    {66.68f, 83.35f, 100.0f} // Plenty of Water
  };

  // Rule Water Sprinkling
  // Temperature x Humidity
  // 0: No Water
  // 1: Less Water
  // 2: Medium Water
  // 3: More Water
  // 4: Plenty of Water
  byte rule_water[25] = {
    2, 1, 0, 0, 0,
    2, 2, 0, 0, 0,
    3, 2, 1, 0, 0,
    3, 3, 2, 1, 0,
    4, 3, 2, 2, 1
  };

  // Fuzzification
  float ling_temp[5], ling_hum[5];
  for (byte i = 0; i < 5; i++) {
    ling_temp[i] = MAX(MIN((temp - imf_temp[i][0]) / (imf_temp[i][1] - imf_temp[i][0]), (imf_temp[i][2] - temp) / (imf_temp[i][2] - imf_temp[i][1])), 0);
    ling_hum[i] = MAX(MIN((humidity - imf_hum[i][0]) / (imf_hum[i][1] - imf_hum[i][0]), (imf_hum[i][2] - humidity) / (imf_hum[i][2] - imf_hum[i][1])), 0);
  }

  // Implication
  float implication[25];
  for (byte i = 0; i < 5; i++) {
    for (byte j = 0; j < 5; j++) {
      implication[5 * i + j] = MIN(ling_temp[i], ling_hum[j]);
    }
  }

  // Aggregation
  float ling_water[5] = {-1, -1, -1, -1, -1};
  for (byte i = 0; i < 5; i++) {
    for (byte j = 0; j < 25; j++) {
      if (rule_water[j] == i) {
        if (ling_water[i] == -1) ling_water[i] = implication[j];
        else ling_water[i] = MAX(ling_water[i], implication[j]);
      }
    }
  }

  // Defuzzification
  float crisp_water[5];
  float out_summary;
  float numerator_cog = 0.0f;
  float denumerator_cog = 0.0f;
  for (byte i = 0; i < 100; i++) {
    for (byte j = 0; j < 5; j++) {
      crisp_water[j] = MIN(ling_water[j], MAX(MIN((i - omf_water[j][0]) / (omf_water[j][1] - omf_water[j][0]), (omf_water[j][2] - i) / (omf_water[j][2] - omf_water[j][1])), 0));
      if (j == 0) out_summary = crisp_water[j];
      else out_summary = MAX(out_summary, crisp_water[j]);
    }
    numerator_cog += (i + 1) * out_summary;
    denumerator_cog += out_summary;
  }

  // Centre of Gravity and Result
  byte cog = numerator_cog / denumerator_cog;

  // Decision
  float out_result[] = {
    MAX(MIN((cog - omf_water[0][0]) / (omf_water[0][1] - omf_water[0][0]), (omf_water[0][2] - cog) / (omf_water[0][2] - omf_water[0][1])), 0),
    MAX(MIN((cog - omf_water[1][0]) / (omf_water[1][1] - omf_water[1][0]), (omf_water[1][2] - cog) / (omf_water[1][2] - omf_water[1][1])), 0),
    MAX(MIN((cog - omf_water[2][0]) / (omf_water[2][1] - omf_water[2][0]), (omf_water[2][2] - cog) / (omf_water[2][2] - omf_water[2][1])), 0),
    MAX(MIN((cog - omf_water[3][0]) / (omf_water[3][1] - omf_water[3][0]), (omf_water[3][2] - cog) / (omf_water[3][2] - omf_water[3][1])), 0),
    MAX(MIN((cog - omf_water[4][0]) / (omf_water[4][1] - omf_water[4][0]), (omf_water[4][2] - cog) / (omf_water[4][2] - omf_water[4][1])), 0)
  };

  byte max_result;
  for (byte i = 0; i < 5; i++) {
    if (i == 0) max_result = i;
    else max_result = out_result[max_result] > out_result[i] ? max_result : i;
  }

  // Returned Result
  switch(max_result){
    case 0: return "No Water"; break;
    case 1: return "Less Water"; break;
    case 2: return "Medium Water"; break;
    case 3: return "More Water"; break;
    case 4: return "Plenty of Water"; break;
    default: return "Error"; break;
  }
}
