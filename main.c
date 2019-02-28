#include <msp430.h> 
#include<math.h>
float predSOC2;
float predV12;
float dt;
float g,Ibatt;
float R1,C2,R2,C1;
float dSOC1p,dV1p;
float V1,prevV1,prevadcV1,prevdV1;
float V2,prevV2;
float R_V12,R_SOC2;
float KG_SOC2,KG_V12;
float Ibatt,prevIbatt,Vmodel,prevVmodel;
float SOC2,V2;

float prevSOC;
float prevdSOC;
float P2_SOC,P2_V1,V12;
float predSOC,predV1;
float dSOC,dV1;
float R_V1,R_SOC;
float KG_SOC,KG_V1;
float P_dSOC,P_dV1;
float SOC;
int c=1;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    if(c==1)
    {
        prevVmodel=Vmodel;
        prevV2=V2;
        prevIbatt=Ibatt;
    }
    if(c==2)
    {
    V1=Vmodel-V2-Ibatt*R1;
    prevadcV1=Vmodel-prevV2-prevIbatt*R1;
    //For X2p calculation
    predSOC2=Ibatt*(-dt/(g*C2));
    predV12=V1*(1 - (dt/(R2*C1))) + Ibatt*(dt/C1);

    //For P2p calculation(predicted value error matrix)
	dSOC1p=predSOC2;
	dV1p=(V1-prevadcV1)*(1-(dt/R2*C1));

	//Calculation of R(error in measured values matrix)
	R_V12=Vmodel-prevVmodel-V2+prevV2-R1*(Ibatt-prevIbatt);
	R_SOC2=(V2-prevV2)/g;

	//For KG calculation
	KG_SOC2=(dSOC1p*dSOC1p)/((dSOC1p*dSOC1p)+(R_SOC2*R_SOC2));
	KG_V12=(dV1p*dV1p)/((dV1p*dV1p)+(R_V12*R_V12));


	//For X2 calculation
	SOC2= predSOC2 + KG_SOC2*(Vmodel-g*predSOC2-predV12-R1*Ibatt);
	V12=predV2 + KG_V12*(Vmodel-g*predSOC2-predV12-R1*Ibatt);

	//For P2 calculation
	P2_SOC=(1-KG_SOC2)*(dSOC1p*dSOC1p);
	P2_V1=(1-KG_V12)*(dV1p*dV1p);

	//Update values.
	prevSOC=SOC2;
	prevV1=V12;
	prevdSOC=sqrt(P2_SOC);
	prevdV1=sqrt(P2_V1);
    }

    else
    {
        //For  Xkp calculation
        predSOC=prevSOC-Ibatt*(dt/(g*C2));
        predV1=prevV1*(1-dt/(R2*C1)) + Ibatt*(dt/C1);

        //For Pkp calculation
        dSOC=prevdSOC;
        dV1=prevdV1*(1-(dt/R2*C1));

        //For R calculation
        R_V1=Vmodel-prevVmodel-V2+prevV2-R1*(Ibatt-prevIbatt);
        R_SOC=(V2-prevV2)/g;

        //For KG calculation
        KG_SOC=(dSOC*dSOC)/((dSOC*dSOC)+(R_SOC*R_SOC));
        KG_V1=(dV1*dV1)/((dV1*dV1)+(R_V1*R_V1));

        //For SOC calculation
        SOC= predSOC + KG_SOC*(Vmodel-predV1-R1*Ibatt-g*predSOC);
        V1=predV1 + KG_V1*(Vmodel-predV1-R1*Ibatt-g*predSOC);

        //For P calculation
        P_dSOC=(1-KG_SOC)*(dSOC*dSOC);
        P_dV1=(1-KG_V1)*(dV1*dV1);

        //Update values.
        prevSOC=SOC;
        prevV1=V1;
        prevdSOC=sqrt(P_dSOC);
        prevdV1=sqrt(P_dV1);
        prevVmodel=Vmodel;
        prevIbatt=Ibatt;
        prevV2=V2;
    }
    c++;
	return 0;
}
