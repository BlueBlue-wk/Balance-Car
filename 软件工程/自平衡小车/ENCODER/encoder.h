#ifndef __ENCODER_H
#define __ENCODER_H	

typedef struct
{
	float Speed_Left;
	float Speed_Right;
}ENCODER;

void Encoder_Init(void);
void Encode_Read(ENCODER* Encoder);

#endif
