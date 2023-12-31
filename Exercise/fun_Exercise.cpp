//Exercises: AB models
   
#include "fun_head_fast.h"
   
MODELBEGIN
   
EQUATION("Price")
/*
The price of the firms as a weighted average between the desired price and the market average price in the previous period.
*/
 v[0] = V("weight");
 v[1] = V("Desired");
 v[2] = AVEL("Price", 1);
 v[3] = v[0]*v[1] + (1-v[0])*v[2];
RESULT(v[3] )
  
EQUATION("Desired")
/*
This equation represents the desired price of firm i in period t, that is given by the current unit cost adjusted by a fixed markup.
*/
 v[0] = V("Cost");
 v[1] = V("markup");
 v[2] = (1+v[1])*v[0];
  
RESULT(v[2] )
EQUATION("Cost")
/*
The cost is given by a normal distribution using the parameters avg and sd.
*/
 v[0] = V("avg");
 v[1] = V("sd");
 v[2]=norm(v[0], v[1]);
  
RESULT(v[2] )

EQUATION("Avg_Price")
/*
Use already implemented macros to calculate the average price:
*/

v[0]=0;
v[2]=0;
CYCLE(cur, "Firm")
{
	v[1] = VS(cur,"Price");
	v[0] = v[0] + v[1];  // sum of prices
	v[2] = v[2] + 1;     // count firms  
}

if(v[2]!=0)
v[3] = v[0]/v[2];
else
v[3]=0;

RESULT(v[3])

EQUATION("Max_Value")
/*
The function calculates the maximum price for a given period.
*/
 v[0]=0;
 CYCLE(cur, "Firm")
 {
   v[1] = VS(cur,"Price");
   if(v[1] > v[0])
   v[0] = v[1];
 }
RESULT(v[0])  

EQUATION("Quality")
/*
The function generates a quality 'factor' associated with the firm.
*/

v[0] = V("RandomQ");
v[1] = VL("Quality", 1);
v[2] = v[1] + v[2];

RESULT(v[2])

EQUATION("RandomQ")
/*
The function generates a random quality component based on a uniform distribution.
*/

v[0] = V("qmin");
v[1] = V("qmax");
v[2] = uniform(v[0], v[1]);

RESULT(v[2])

EQUATION("Competitiveness")
/*
The function calculates competitiveness based on price, quality and elasticities.
*/

v[0] = V("elasticityQ");
v[1] = V("elasticityP");

v[2] = pow(V("Quality"),v[0]);
v[3] = pow(V("Price"),v[1]); 

if(v[3]==0)
v[4] = 0;
else
v[4] = v[2]/v[3];

RESULT(v[4])

EQUATION("Ave_Comp")
/*
The function calculates the average competitiveness over a period.
*/

v[0] = AVE("Competitiveness");

RESULT(v[0])

EQUATION("Market_share")
/*
The function calculates the market share of firm i in period t.
*/

v[0] = VL("Market_share",1);
v[1] = V("adjustment"); //?j is market share adjustment parameter of sector j and 0 < ?j < 1
v[2] = V("Competitiveness");
v[3] = V("Ave_Comp");

v[4] = v[0]*(1+v[1]*((v[2]/v[3])-1));

RESULT(v[4])

EQUATION("ConsistencyT")
/*
The function create a variable to check the consistency of the model.
*/

if(SUM("Market_share") == 1)
v[0] = 0;
else
v[0] = 1;

RESULT(v[0])

EQUATION("SumMarketS")
/*
Soma dos market shares
*/

v[0] = SUM("Market_share");

RESULT(v[0])


EQUATION("Normalized")
/*
The function implement a normalization procedure inside the variable that sums the market share.
*/
v[0] = V("SumMarketS");
v[1] = V("Market_share");
v[2] = v[1]/v[0];
WRITE("Market_share",v[2]);

RESULT(v[2])

EQUATION("SumNormalized")
/*
Teste da soma normalizada.
*/

v[0] = SUM("Normalized");

RESULT(v[0])

EQUATION("Exit")
/*
Remove a firma com market share abaixo de um determinado limite.
*/
v[0]=0;
v[2]=0;
v[3] = V("threshold");
CYCLE(cur, "Firm")
{
	v[1] = VS(cur,"Market_share");
	if(v[1] < v[3])
	DELETE(cur);
}

RESULT(0)

EQUATION("CountFirms")
/*
Conta o n�mero de firmas.
*/

v[0]=0;
CYCLE(cur, "Firm")
{
	v[0] = v[0] + 1;     // count firms  
}

RESULT(v[0])

EQUATION("Entry")
v[1] = V("switch_entry"); //0 = no entry, 1 = max market share, 2 = random firm


if(v[1] ==1);
{
v[0] = MAX("Market_share");
cur = SEARCH_CND("Market_share",v[0]);
ADDOBJ_EX("Firm",cur);
cur2 = ADDOBJ_EX("Firm",cur);
WRITES(cur2,"markup", 0.1);
}

if(v[2] == 2)
{
cur = RNDDRAW_FAIR("Firm");
ADDOBJ_EX("Firm",cur);
}

RESULT(0)

EQUATION("HHI")
/*
Inverse HHI variable at the sectoral level to capture the concentration of the sector
*/

v[0]=0;
CYCLE(cur, "Firm")
{
    v[1] = pow(VS(cur,"Market_share"),2);
	v[0] = v[0] + v[1];     
}

v[2] = 1/v[0];

RESULT( v[2])

EQUATION("Ave_HHI")
/*
The function calculates the average HHI
*/

v[0] = AVE("HHI");

RESULT(v[0])


  
MODELEND
// do not add Equations in this area
void close_sim( void )
{
// close simulation special commands go here
} 
