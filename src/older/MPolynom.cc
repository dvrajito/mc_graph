#include "GenTypes.h"
#include "MPolynom.h"

Polynom *NewPolynom(int aGrade)
{ Polynom *p;
  int i;

  p = (Polynom *)malloc(sizeof(Polynom));
  p->grade = aGrade;
  p->coefs = (double *) malloc(aGrade * sizeof(double));
  for (i=0; i<aGrade; i++)
    p->coefs[i] = 0;
  return p;
}

double EvalPol(Polynom *p, double x)
{ int i;
  double result = 0, xPower = 1;
  
  for (i=0; i<p->grade; i++)
  { result += xPower * p->coefs[i];
    xPower *= x;
  }
  return result;
}

Polynom *Primitive(Polynom *p)
{ Polynom *prim;
  int i;
  
  prim = NewPolynom(p->grade + 1);
  for (i=1; i<=p->grade; i++)
    prim->coefs[i] = (1/i) * p->coefs[i-1];
  return prim;
}

double DeltaP(Polynom *p, double inf, double sup)
{
  return EvalPol(p, sup) - EvalPol(p, inf);
}

double IntegralMod(Polynom *p, Polynom *prim, double inf, double sup)
{ double interm, result = 0;

  if (sup - inf <= 4)
    for (interm=inf; interm<sup; interm += 1)
      result += fabs(DeltaP(prim, interm, (double) (interm+1)));
  else
  { interm = (sup + inf)/2;
    if (Eval(p, inf)*Eval(p, interm) >= 0)
      result += fabs(DeltaP(prim, inf, interm));
    else
      result += IntegralMod(p, prim, inf, interm);
    if (Eval(p, sup)*Eval(p, interm) >= 0)
      result += fabs(DeltaP(prim, interm, sup));
    else
      result += IntegralMod(p, prim, interm, sup);
  }
}

double Distance(Polynom *p1, Polynom *p2, double inf, double sup)
{ double result;
  Polynom *prim, *diff;
  int i;
  
  diff = NewPolynom(p1->grade);
  for (i=0; i<p1->grade; i++)
    diff->coefs[i] = p2->coefs[i] - p1->coefs[i];
  prim = Primitive(diff);
  return IntegralMod(diff, prim, inf, sup);
}


