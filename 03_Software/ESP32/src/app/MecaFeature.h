#ifndef __MECA_FEATURE_H_
#define __MECA_FEATURE_H_

#define WORM_SCREW_2MM
#ifdef WORM_SCREW_2MM
    /* if you have 2mm worm screw */
    #define FREQ_FOR_WORM_SCREW_WITH_CUTTER     160
    #define FREQ_FOR_WORM_SCREW_WITHOUT_CUTTER  2000
#else
    /* if you have 8mm worm screw */
    #define FREQ_FOR_WORM_SCREW_WITH_CUTTER     40
    #define FREQ_FOR_WORM_SCREW_WITHOUT_CUTTER  500
#endif

#endif /* __MECA_FEATURE_H_ */
