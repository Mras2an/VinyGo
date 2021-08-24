#ifndef CODE_HARDWAREV2_SRC_APP_SPEEDVINYL_SPEEDVINYL_H_
#define CODE_HARDWAREV2_SRC_APP_SPEEDVINYL_SPEEDVINYL_H_

void SpeedVinyl_init();

int SpeedVinyl_get();

void SpeedVinyl_set(int val);

void SpeedVinyl_getString(char * buffer);

void SpeedVinyl_get33or45String(char * buffer);

void SpeedVinyl_getAllString(char * buffer);

void SpeedVinyl_increase();

void SpeedVinyl_decrease();
//
//void SpeedVinyl_set33or45(int val);
//
//void SpeedVinyl_increase33or45();
//
//void SpeedVinyl_decrease33or45();
//
//int SpeedVinyl_get33or45();

#endif /* CODE_HARDWAREV2_SRC_APP_SPEEDVINYL_SPEEDVINYL_H_ */
