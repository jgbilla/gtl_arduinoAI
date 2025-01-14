#include <MicroTFLite.h>
#include "model.h"

constexpr int kTensorArenaSize = 20 * 1024;
alignas(16) byte tensor_arena[kTensorArenaSize];

void setup(){
  Serial.begin(115200);
  while (!Serial)
        ;
  
  delay(1000);

  if ((uint32_t)tensor_arena % 16 != 0) {
        Serial.println("Error: Tensor arena not properly aligned!");
        while (true);
    }
  //Initialize the model
  if(!ModelInit(model, tensor_arena, kTensorArenaSize)){
    Serial.println("Model initialization failed!");
    while (true);
  }
  Serial.println("Model initialization done.");

   
  ModelPrintTensorInfo();  // Print tensor types and dimensions
  ModelPrintTensorQuantizationParams();  // Print quantization parameters
  ModelPrintInputTensorDimensions();
  ModelPrintOutputTensorDimensions();
  ModelPrintMetadata();

  Serial.println("");
  Serial.println("Please input 5 number that either are incremented by a constant, or a Fibonnaci sequence, or prime numbers");
}

void loop(){
  float input_sequence[] = {5,10,15,20,25};
  bool inputSetSuccess = true;
  for(int i = 0; i < 5; i++) {
    if(!ModelSetInput(input_sequence[i],i,true)){
      inputSetSuccess = false;
    }
  }
  
  ModelPrintTensorInfo();
  Serial.print("Setting success: ");
  Serial.println(inputSetSuccess ? "Yes" : "No");

  if (!ModelRunInference()) {
        Serial.println("Run Inference Failed!");
        return;
    }
    // bool success = ModelRunInference();
    // Serial.print("Inference success: ");
    // Serial.println(success ? "Yes" : "No");

  
    // Get and print raw probabilities with high precision
    Serial.println("\nClass probabilities:");
    float max_prob = 0.0f;
    int predicted_class = -1;
    
    for(int i = 0; i < 3; i++) {
        Serial.println(ModelGetOutput(i));
        float prob = ModelGetOutput(i);
        Serial.print("Class ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(ModelGetOutput(i));  // Print with 8 decimal places
        
        // Track the highest probability
        if (prob > max_prob) {
            max_prob = prob;
            predicted_class = i;
        }
    }

    // Print the prediction
    Serial.print("\nPredicted sequence type: ");
    switch(predicted_class) {
        case 0:
            Serial.println("Arithmetic Progression");
            break;
        case 1:
            Serial.println("Fibonnaci numbers");
            break;
        case 2:
            Serial.println("Prime Numbers");
            break;
        default:
            Serial.println("Unknown");
    }
 
    delay(2000);
}