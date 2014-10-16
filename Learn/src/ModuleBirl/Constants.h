
/* BIRL constants */



// birl inputs

#define KEYS_NUMBER 9
#define PRESSURE_NUMBER 2
#define EMBOUCHURE_NUMBER 20
#define EMBOUCHURE_MAX 4096
#define KEYS_MAX 1500
#define KEYS_DISCRETE_THRESHOLD 0.5


// learning

#define LEARN_TYPE LearnOutputParameter::MLP
#define MLP_HIDDEN_LAYERS 2
#define MLP_TARGET RMSE 0.001
#define MLP_MAX_SAMPLES 1000000


// drawing

#define BIRL_DRAW_EMBOUCHURE_H 80
#define BIRL_DRAW_PRESSURE_H 40
