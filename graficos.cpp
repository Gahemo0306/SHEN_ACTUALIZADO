#include "graficos.h"
#include <cmath>


Graficos::Graficos()
{

}

Plot_curvascompuestas::Plot_curvascompuestas(QVector<double> Tsupply, QVector<double> Ttarget,
                                             QVector<double> Cp)
{
    QVector<double> VCC ,VFC, VectorCalientes, VectorCalientesCp;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas;
    int i,j,n,nfils,ncols,Pares,contador,k;
    n = Tsupply.size();
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    QVector<QVector <double>> VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(int j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    VCC = VectorCalientes;
    VFC = VectorFrias;
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    //qDebug() << VectorCalientes << VectorFrias;
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    VectorCalientes = VectorCalientesNuevo;
    VectorFrias = VectorFriasNuevo;
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size();
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l=0;
    int m =VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0] - .1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-.1 )
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size();
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-.1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-.1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    // sumas de vector para sumCpFri y sumCpCal
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val1;
    n = CpcurvaCold.size();
    for(i=0; i < n ; i++){
        Val1 << RangosFrias[i][2] * CpcurvaCold[i];
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    QVector<double> PuntosCurvasC,PuntosCurvasF;
    n = PUNTOSCC.size();
    PuntosCurvasC.resize(PUNTOSCC.size()+1);
    PuntosCurvasC[0] = 0;
    for(i = 0 ; i < n; i++){
        PuntosCurvasC[i+1] = PuntosCurvasC[i] + PUNTOSCC[i][3];
    }
    n = PUNTOSCF.size();
    PuntosCurvasF.resize(PUNTOSCF.size()+1);
    PuntosCurvasF[0] = 0;
    for(i = 0; i < n; i++){
        PuntosCurvasF[i+1] = PuntosCurvasF[i] + PUNTOSCF[i][3];
    }
    MCCENTALPIA.resize(PuntosCurvasC.size());
    MCCTEMPERATURAS.resize(VCCurvas.size());
    for ( int i = 0 ; i < PuntosCurvasC.size() ; i++){
        MCCENTALPIA[i] = PuntosCurvasC[i];
        MCCTEMPERATURAS[i] = VCCurvas[i];
    }
    MCFENTALPIA.resize(PuntosCurvasF.size());
    MCFTEMPERATURAS.resize(VFCurvas.size());
    for ( int i = 0 ; i < PuntosCurvasF.size() ; i++){
        MCFENTALPIA[i] = PuntosCurvasF[i];
        MCFTEMPERATURAS[i] = VFCurvas[i];
    }
}

QVector<double> Plot_curvascompuestas::getCCENTALPIA()
{
    return MCCENTALPIA;
}

QVector<double> Plot_curvascompuestas::getCCTEMPERATURAS()
{
    return MCCTEMPERATURAS;
}

QVector<double> Plot_curvascompuestas::getCFENTALPIA()
{
    return MCFENTALPIA;
}

QVector<double> Plot_curvascompuestas::getCFTEMPERATURAS()
{
    return MCFTEMPERATURAS;
}

Plot_curvascompuestasajustadas::Plot_curvascompuestasajustadas(QVector<double> Tsupply, QVector<double> Ttarget, QVector<double> Cp, double DTmin)
{
    QVector<double> VectorCalientes, VectorCalientesCp;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas;
    int i,j,n,nfils,ncols,Pares,contador,k;
    n = Tsupply.size();
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    QVector < QVector <double> > VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorCalientesMATRIZ.resize(nfils);
    MVectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        MVectorCalientesMATRIZ[i].resize(ncols);
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            MVectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorFriasMATRIZ.resize(nfils);
    MVectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
        MVectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            MVectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VCC,VFC;
    Pares = VectorCalientes.size();
    nfils = Pares/2;
    ncols = 2;
    VCC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VCC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VCC[i][0] = VectorCalientes[k];
        k++;
        VCC[i][1] = VectorCalientes[k];
        k++;
    }
    Pares = VectorFrias.size();
    nfils = Pares/2;
    ncols = 2;
    VFC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VFC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VFC[i][0] = VectorFrias[k];
        k++;
        VFC[i][1] = VectorFrias[k];
        k++;
    }
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    QVector<double> VCCurvasT = VCCurvas;
    QVector<double> VFCurvasT = VFCurvas;
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size();//n; //corregido
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l = 0;
    int m = VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0]-.1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-.1)
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size();
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-.1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-.1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    //MODIFICADO
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val1;
    n = CpcurvaCold.size();
    for(i=0; i < n ; i++){
        Val1 << RangosFrias[i][2] * CpcurvaCold[i];
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    // HASTA AQUI ES PARA CURVAS COMPUESTAS
    QVector<QVector<double>> VectorCalientesTmin;
    VectorCalientesTmin.resize(VCCurvasT.size());
    nfils = VCCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i].resize(ncols);
    }
    k = VCCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i][0] = VCCurvasT[k]-DTmin;
        VectorCalientesTmin[i][1] = VCCurvasT[k];
        --k;
    }
    QVector<QVector<double>> VectorFriasTmin;
    VectorFriasTmin.resize(VFCurvasT.size());
    nfils = VFCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i].resize(ncols);
    }
    k = VFCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i][0] = VFCurvasT[k];
        VectorFriasTmin[i][1] = VFCurvasT[k]+DTmin;
        --k;
    }
    QVector<QVector<double>> VectorCorrientesTotal;
    nfils = VCCurvasT.size() + VFCurvasT.size();
    ncols = 2;
    VectorCorrientesTotal.resize(nfils);
    MVectorCorrientesTotal.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotal[i].resize(ncols);
        MVectorCorrientesTotal[i].resize(ncols);
    }
    j=0;
    k=0;
    for(i = 0; i < nfils ; i++){
        if(i < VectorCalientesTmin.size()){
            VectorCorrientesTotal[i][0] = VectorCalientesTmin[j][0];
            VectorCorrientesTotal[i][1] = VectorCalientesTmin[j][1];
            ++j;
        }else{
            VectorCorrientesTotal[i][0] = VectorFriasTmin[k][0];
            VectorCorrientesTotal[i][1] = VectorFriasTmin[k][1];
            ++k;
        }
    }
    std::sort(VectorCorrientesTotal.begin(),VectorCorrientesTotal.end());
    QVector<QVector<double>> VectorCorrientesTotalOrdenar;
    nfils = VectorCorrientesTotal.size();
    ncols = 2;
    VectorCorrientesTotalOrdenar.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i].resize(ncols);
    }
    k = VectorCorrientesTotal.size()-1;
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i][0] = VectorCorrientesTotal[k][0];
        VectorCorrientesTotalOrdenar[i][1] = VectorCorrientesTotal[k][1];
        --k;
    }
    VectorCorrientesTotal = VectorCorrientesTotalOrdenar;
    MVectorCorrientesTotal = VectorCorrientesTotal;
    int r3 = VectorCorrientesTotal.size();
    int r4 = VCC.size();
    //FORMAR LA MATRIZ PARA SUMCPCAL
    QVector<QVector<double>> sumCpCalTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VCC.size();//2;
    sumCpCalTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpCalTab[i].resize(ncols);
    }
    l = 0;
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r4; ++j){
            if(VectorCorrientesTotal[i][1] <= VCC[j][0]-.1 && VectorCorrientesTotal[i][1] >= VCC[j][1]-.1 ){
                sumCpCalTab[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCalTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<QVector<double>> sumCpFriTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VFC.size();//2;
    sumCpFriTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpFriTab[i].resize(ncols);
    }
    l = 0;
    int r5 = VFC.size();
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r5; ++j){
            if(VectorCorrientesTotal[i][0] <= VFC[j][1]-.1 && VectorCorrientesTotal[i][0] >= VFC[j][0]-.1 ){
                sumCpFriTab[l][k] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFriTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<double> SumatoriaCP,SumatoriaCPCAL,SumatoriaCPFRI;
    nfils = VectorCorrientesTotal.size()-1;
    SumatoriaCP.resize(nfils);
    SumatoriaCPCAL.resize(nfils);
    SumatoriaCPFRI.resize(nfils);
    for(i = 0; i < nfils ; i++){
        SumatoriaCPCAL[i] = 0;
        for(j = 0 ; j < sumCpCalTab[0].size();j++){
            SumatoriaCPCAL[i] = SumatoriaCPCAL[i] + sumCpCalTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCPFRI[i] = 0;
        for(j = 0 ; j < sumCpFriTab[0].size(); j++){
            SumatoriaCPFRI[i] = SumatoriaCPFRI[i] + sumCpFriTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCP[i] = SumatoriaCPFRI[i] - SumatoriaCPCAL[i] ;
    }
    QVector<double> DifTemperaturas;
    nfils = VectorCorrientesTotal.size()-1;
    DifTemperaturas.resize(nfils);
    j = 1;
    for(i = 0; i < VectorCorrientesTotal.size() ; ++i){
        if (j < VectorCorrientesTotal.size()){
            DifTemperaturas[i] = VectorCorrientesTotal[i][1] - VectorCorrientesTotal[j][1];
            ++j;
        }else if(j == VectorCorrientesTotal.size()){
            break;
        }
    }
    QVector<double> Deficit;
    nfils = DifTemperaturas.size();
    Deficit.resize(DifTemperaturas.size());
    MDeficit.resize(DifTemperaturas.size());
    for( i = 0 ; i < nfils; ++i){
        Deficit[i] = DifTemperaturas[i] * SumatoriaCP[i];
        MDeficit[i] = Deficit[i];
    }
    int Ldef = Deficit.size();
    QVector<QVector<double>> VecHeatFlow;
    QVector<double> VAL1,VAL2;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecHeatFlow.resize(nfils);
    MVecHeatFlow.resize(nfils);
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecHeatFlow[i].resize(ncols);
        MVecHeatFlow[i].resize(ncols);
    }
    VAL1[0]=0;
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecHeatFlow[i][0] = VAL1[i]; // Val1
        VecHeatFlow[i][1] = VAL2[i]; // Val2
        MVecHeatFlow[i][0] = VecHeatFlow[i][0];
        MVecHeatFlow[i][1] = VecHeatFlow[i][1];
    }
    double min = *std::min_element(VAL2.begin(),VAL2.end());
    VAL1.clear();
    VAL2.clear();
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    QVector<QVector<double>> VecAdjHeatFlow;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecAdjHeatFlow.resize(nfils);
    MVecAdjHeatFlow.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecAdjHeatFlow[i].resize(ncols);
        MVecAdjHeatFlow[i].resize(ncols);
    }
    VAL1[0]= qFabs(min);
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecAdjHeatFlow[i][0] = VAL1[i]; // Val1
        VecAdjHeatFlow[i][1] = VAL2[i]; // Val2 ;
        MVecAdjHeatFlow[i][0] = VecAdjHeatFlow[i][0];
        MVecAdjHeatFlow[i][1] = VecAdjHeatFlow[i][1];
    }
    // PUNTOS DE LA CURVAS COMPUESTAS
    QVector<double> PuntosCurvasC,PuntosCurvasF;
    n = PUNTOSCC.size();
    PuntosCurvasC.resize(PUNTOSCC.size()+1);
    PuntosCurvasC[0] = 0;
    for(i = 0 ; i < n; i++){
        PuntosCurvasC[i+1] = PuntosCurvasC[i] + PUNTOSCC[i][3];
    }
    n = PUNTOSCF.size();
    PuntosCurvasF.resize(PUNTOSCF.size()+1);
    //double max = *std::max_element(VAL2.begin(),VAL2.end());
    double max = VecAdjHeatFlow[VecAdjHeatFlow.size()-1][1];
    PuntosCurvasF[0] = max ;
    for(i = 0; i < n; i++){
        PuntosCurvasF[i+1] = PuntosCurvasF[i] + PUNTOSCF[i][3];
    }
    MCCENTALPIA.resize(PuntosCurvasC.size());
    MCCTEMPERATURAS.resize(VCCurvas.size());
    for ( int i = 0 ; i < PuntosCurvasC.size() ; i++){
        MCCENTALPIA[i] = PuntosCurvasC[i];
        MCCTEMPERATURAS[i] = VCCurvas[i];
    }
    MCFENTALPIAAJUSTADA.resize(PuntosCurvasF.size());
    MCFTEMPERATURASAJUSTADA.resize(VFCurvas.size());
    for ( int i = 0 ; i < PuntosCurvasF.size() ; i++){
        MCFENTALPIAAJUSTADA[i] = PuntosCurvasF[i];
        MCFTEMPERATURASAJUSTADA[i] = VFCurvas[i];
    }
}

QVector<double> Plot_curvascompuestasajustadas::getCCENTALPIA()
{
    return MCCENTALPIA;
}

QVector<double> Plot_curvascompuestasajustadas::getCCTEMPERATURAS()
{
    return MCCTEMPERATURAS;
}

QVector<double> Plot_curvascompuestasajustadas::getCFENTALPIAAJUSTADAS()
{
    return MCFENTALPIAAJUSTADA;
}

QVector<double> Plot_curvascompuestasajustadas::getCFTEMPERATURASAJUSTADAS()
{
    return MCFTEMPERATURASAJUSTADA;
}

QVector<QVector<double> > Plot_curvascompuestasajustadas::getVectorCorrientesTotal()
{
    return MVectorCorrientesTotal;
}

QVector<QVector<double> > Plot_curvascompuestasajustadas::getVecHeatFlow()
{
    return MVecHeatFlow;
}

QVector<QVector<double> > Plot_curvascompuestasajustadas::getVecAdjHeatFlow()
{
    return MVecAdjHeatFlow;
}

QVector<double> Plot_curvascompuestasajustadas::getDeficit()
{
    return MDeficit;
}

QVector<QVector<double> > Plot_curvascompuestasajustadas::getVectorCalientesMATRIZ()
{
    return MVectorCalientesMATRIZ;
}

QVector<QVector<double> > Plot_curvascompuestasajustadas::getVectorFriasMATRIZ()
{
    return MVectorFriasMATRIZ;
}

Plot_Dtmin_vs_Areas::Plot_Dtmin_vs_Areas(QVector<double> Tsupply, QVector<double> Ttarget,
                                         QVector<double> Cp, QVector<double> h,
                                         QVector<double> Calentamiento, QVector<double> Enfriamento,
                                         double DTmin, int CTo, int CCo)
{
    QVector<double> VectorCalientes, VectorCalientesCp;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas;
    int i,j,n,nfils,ncols,Pares,contador,k;
    n = Tsupply.size();
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] << h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] << h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    QVector < QVector <double> > VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/4;
    ncols = 4;
    VectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/4;
    ncols = 4;
    VectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VCC,VFC;
    Pares = VectorCalientes.size();
    nfils = Pares/2;
    ncols = 2;
    VCC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VCC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VCC[i][0] = VectorCalientes[k];
        k++;
        VCC[i][1] = VectorCalientes[k];
        k++;
    }
    Pares = VectorFrias.size();
    nfils = Pares/2;
    ncols = 2;
    VFC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VFC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VFC[i][0] = VectorFrias[k];
        k++;
        VFC[i][1] = VectorFrias[k];
        k++;
    }
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    QVector<double> VCCurvasT = VCCurvas;
    QVector<double> VFCurvasT = VFCurvas;
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size();
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l=0;
    int m =VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0]-.1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-.1 )
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size();
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-.1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-.1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    //MODIFICADO
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val1;
    n = CpcurvaCold.size();
    for(i=0; i < n ; i++){
        Val1 << RangosFrias[i][2] * CpcurvaCold[i];
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    // HASTA AQUI ES PARA CURVAS COMPUESTAS
    QVector<QVector<double>> VectorCalientesTmin;
    VectorCalientesTmin.resize(VCCurvasT.size());
    nfils = VCCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i].resize(ncols);
    }
    k = VCCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i][0] = VCCurvasT[k]-DTmin;
        VectorCalientesTmin[i][1] = VCCurvasT[k];
        --k;
    }
    QVector<QVector<double>> VectorFriasTmin;
    VectorFriasTmin.resize(VFCurvasT.size());
    nfils = VFCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i].resize(ncols);
    }
    k = VFCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i][0] = VFCurvasT[k];
        VectorFriasTmin[i][1] = VFCurvasT[k]+DTmin;
        --k;
    }
    QVector<QVector<double>> VectorCorrientesTotal;
    nfils = VCCurvasT.size() + VFCurvasT.size();
    ncols = 2;
    VectorCorrientesTotal.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotal[i].resize(ncols);
    }
    j=0;
    k=0;
    for(i = 0; i < nfils ; i++){
        if(i < VectorCalientesTmin.size()){
            VectorCorrientesTotal[i][0] = VectorCalientesTmin[j][0];
            VectorCorrientesTotal[i][1] = VectorCalientesTmin[j][1];
            ++j;
        }else{
            VectorCorrientesTotal[i][0] = VectorFriasTmin[k][0];
            VectorCorrientesTotal[i][1] = VectorFriasTmin[k][1];
            ++k;
        }
    }
    std::sort(VectorCorrientesTotal.begin(),VectorCorrientesTotal.end());
    QVector<QVector<double>> VectorCorrientesTotalOrdenar;
    nfils = VectorCorrientesTotal.size();
    ncols = 2;
    VectorCorrientesTotalOrdenar.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i].resize(ncols);
    }
    k = VectorCorrientesTotal.size()-1;
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i][0] = VectorCorrientesTotal[k][0];
        VectorCorrientesTotalOrdenar[i][1] = VectorCorrientesTotal[k][1];
        --k;
    }
    VectorCorrientesTotal = VectorCorrientesTotalOrdenar;
    int r3 = VectorCorrientesTotal.size();
    int r4 = VCC.size();
    //FORMAR LA MATRIZ PARA SUMCPCAL
    QVector<QVector<double>> sumCpCalTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VCC.size();//2;
    sumCpCalTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpCalTab[i].resize(ncols);
    }
    l = 0;
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r4; ++j){
            if(VectorCorrientesTotal[i][1] <= VCC[j][0]-.1 && VectorCorrientesTotal[i][1] >= VCC[j][1] -.1 ){
                sumCpCalTab[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCalTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<QVector<double>> sumCpFriTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VFC.size();//2;
    sumCpFriTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpFriTab[i].resize(ncols);
    }
    l = 0;
    int r5 = VFC.size();
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r5; ++j){
            if(VectorCorrientesTotal[i][0] <= VFC[j][1]-.1 && VectorCorrientesTotal[i][0] >= VFC[j][0] -.1){
                sumCpFriTab[l][k] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFriTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<double> SumatoriaCP,SumatoriaCPCAL,SumatoriaCPFRI;
    nfils = VectorCorrientesTotal.size()-1;
    SumatoriaCP.resize(nfils);
    SumatoriaCPCAL.resize(nfils);
    SumatoriaCPFRI.resize(nfils);
    //MODIFICADO
    for(i = 0; i < nfils ; i++){
        SumatoriaCPCAL[i] = 0;
        for(j = 0 ; j < sumCpCalTab[0].size();j++){
            SumatoriaCPCAL[i] = SumatoriaCPCAL[i] + sumCpCalTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCPFRI[i] = 0;
        for(j = 0 ; j < sumCpFriTab[0].size(); j++){
            SumatoriaCPFRI[i] = SumatoriaCPFRI[i] + sumCpFriTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCP[i] = SumatoriaCPFRI[i] - SumatoriaCPCAL[i] ;
    }
    QVector<double> DifTemperaturas;
    nfils = VectorCorrientesTotal.size()-1;
    DifTemperaturas.resize(nfils);
    j = 1;
    for(i = 0; i < VectorCorrientesTotal.size() ; ++i){
        if (j < VectorCorrientesTotal.size()){
            DifTemperaturas[i] = VectorCorrientesTotal[i][1] - VectorCorrientesTotal[j][1];
            ++j;
        }else if(j == VectorCorrientesTotal.size()){
            break;
        }
    }
    QVector<double> Deficit;
    nfils = DifTemperaturas.size();
    Deficit.resize(DifTemperaturas.size());
    for( i = 0 ; i < nfils; ++i){
        Deficit[i] = DifTemperaturas[i] * SumatoriaCP[i];
    }
    int Ldef = Deficit.size();
    QVector<QVector<double>> VecHeatFlow;
    QVector<double> VAL1,VAL2;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecHeatFlow.resize(nfils);
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecHeatFlow[i].resize(ncols);
    }
    VAL1[0]=0;
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecHeatFlow[i][0] = VAL1[i]; // Val1
        VecHeatFlow[i][1] = VAL2[i]; // Val2
    }
    double min = *std::min_element(VAL2.begin(),VAL2.end());
    VAL1.clear();
    VAL2.clear();
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    QVector<QVector<double>> VecAdjHeatFlow;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecAdjHeatFlow.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecAdjHeatFlow[i].resize(ncols);
    }
    VAL1[0]= qFabs(min);
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecAdjHeatFlow[i][0] = VAL1[i]; // Val1
        VecAdjHeatFlow[i][1] = VAL2[i]; // Val2
    }
    QVector<double> PuntosCurvasC,PuntosCurvasF;
    n = PUNTOSCC.size();
    PuntosCurvasC.resize(PUNTOSCC.size()+1);
    PuntosCurvasC[0] = 0;
    for(i = 0 ; i < n; i++){
        PuntosCurvasC[i+1] = PuntosCurvasC[i] + PUNTOSCC[i][3];
    }
    n = PUNTOSCF.size();
    PuntosCurvasF.resize(PUNTOSCF.size()+1);
    //double max = *std::max_element(VAL2.begin(),VAL2.end());
    double max = VecAdjHeatFlow[VecAdjHeatFlow.size()-1][1];
    PuntosCurvasF[0] = max ;
    for(i = 0; i < n; i++){
        PuntosCurvasF[i+1] = PuntosCurvasF[i] + PUNTOSCF[i][3];
    }
    QVector<QVector<double>> CURVA_COMPUESTA_CALIENTE , CCALIENTE;
    QVector<QVector<double>> CURVA_COMPUESTA_FRIA, CCFRIA;
    nfils = PuntosCurvasC.size();
    ncols = 2;
    CURVA_COMPUESTA_CALIENTE.resize(nfils);
    CCALIENTE.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i].resize(ncols);
        CCALIENTE[i].resize(ncols);
    }
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i][0] = VCCurvas[i];
        CURVA_COMPUESTA_CALIENTE[i][1] = PuntosCurvasC[i];
        CCALIENTE[i][0] = VCCurvas[i];
        CCALIENTE[i][1] = PuntosCurvasC[i];
    }
    nfils = PuntosCurvasF.size();
    ncols = 2;
    CURVA_COMPUESTA_FRIA.resize(nfils);
    CCFRIA.resize(nfils);
    for( i = 0; i < nfils; i++ ){
        CURVA_COMPUESTA_FRIA[i].resize(ncols);
        CCFRIA[i].resize(ncols);
    }
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_FRIA[i][0] = VFCurvas[i];
        CURVA_COMPUESTA_FRIA[i][1] = PuntosCurvasF[i];
        CCFRIA[i][0] = VFCurvas[i];
        CCFRIA[i][1] = PuntosCurvasF[i];
    }
    QVector<QVector<double>> ACON;
    QVector<double> ACON_ENTALPIA;
    nfils = PuntosCurvasC.size() + PuntosCurvasF.size();
    ncols = 2;
    ACON.resize(nfils);
    ACON_ENTALPIA.resize(nfils);
    for(i = 0 ; i < nfils ; i++){
        ACON[i].resize(ncols);
    }
    j=0;k=0;
    for(i = 0; i < nfils ; i++){
        if(i < PuntosCurvasC.size()){
            ACON[i][0] = VCCurvas[j];
            ACON[i][1] = PuntosCurvasC[j];
            ACON_ENTALPIA[i] = PuntosCurvasC[j];
            j++;
        }else{
            ACON[i][0] = VFCurvas[k];
            ACON[i][1] = PuntosCurvasF[k];
            ACON_ENTALPIA[i] = PuntosCurvasF[k];
            k++;
        }
    }
    std::sort(ACON.begin(),ACON.end());
    std::sort(ACON_ENTALPIA.begin(),ACON_ENTALPIA.end());
    QVector<double> Dif_Intervalos_de_Entalpia;
    QVector<double> Intervalos_de_Entalpia;
    QVector<double> VALOR_INI;
    nfils = ACON_ENTALPIA.size();
    VALOR_INI.resize(nfils);
    Dif_Intervalos_de_Entalpia.resize(nfils-1);
    Intervalos_de_Entalpia.resize(nfils-1);
    VALOR_INI[0] = ACON_ENTALPIA[0];
    j=0;
    for ( i = 0 ; i < nfils-1; ++i){
        Dif_Intervalos_de_Entalpia[i] = qFabs(VALOR_INI[i] - ACON_ENTALPIA[i+1]);
        Intervalos_de_Entalpia[j] = Dif_Intervalos_de_Entalpia[i];
        VALOR_INI[i+1] = std::accumulate(Intervalos_de_Entalpia.begin(),Intervalos_de_Entalpia.end(),0);
        ++j;
    }
    QVector<QVector<double>> InterpolacionCalientes;
    int r1 =  CURVA_COMPUESTA_CALIENTE.size();
    int r2 = CURVA_COMPUESTA_FRIA.size();
    for(i = 0; i < r2 ; i++){
        for(k = 0; k < r1 ; k++){
            if(k==(r1-1)){
                break;
            }
            if(CURVA_COMPUESTA_FRIA[i][1] > CURVA_COMPUESTA_CALIENTE[k][1] &&
                    CURVA_COMPUESTA_FRIA[i][1] < CURVA_COMPUESTA_CALIENTE[k+1][1]){
                    InterpolacionCalientes.resize(1);
                    InterpolacionCalientes[0].resize(2);
                    InterpolacionCalientes[0][0] = CURVA_COMPUESTA_CALIENTE[k][0] +
                            ((CURVA_COMPUESTA_CALIENTE[k+1][0]-CURVA_COMPUESTA_CALIENTE[k][0])
                            /(CURVA_COMPUESTA_CALIENTE[k+1][1]-CURVA_COMPUESTA_CALIENTE[k][1]))*
                            (CURVA_COMPUESTA_FRIA[i][1]-CURVA_COMPUESTA_CALIENTE[k][1]);;
                    InterpolacionCalientes[0][1] = CURVA_COMPUESTA_FRIA[i][1] ;
                    CURVA_COMPUESTA_CALIENTE = CURVA_COMPUESTA_CALIENTE + InterpolacionCalientes;
                    InterpolacionCalientes.clear();
                    k = 0;
                    r1 = CURVA_COMPUESTA_CALIENTE.size();
                    j++;
                    break;
            }
        }
    }
    InterpolacionCalientes.resize(CURVA_COMPUESTA_CALIENTE.size());
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        InterpolacionCalientes[i].resize(2);
    }
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_CALIENTE.at(i).size() ; j++){
            InterpolacionCalientes[i][j] = CURVA_COMPUESTA_CALIENTE[i][j];
        }
    }
    CURVA_COMPUESTA_CALIENTE.clear();
    nfils = CCALIENTE.size();
    ncols = 2;
    CURVA_COMPUESTA_CALIENTE.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i].resize(ncols);
    }
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_CALIENTE.at(i).size() ; j++){
             CURVA_COMPUESTA_CALIENTE[i][j] = CCALIENTE[i][j];
        }
    }
    std::sort(InterpolacionCalientes.begin(),InterpolacionCalientes.end());
    QVector<QVector<double>> InterpolacionFrias;
    r1 =  CURVA_COMPUESTA_FRIA.size();
    r2 = CURVA_COMPUESTA_CALIENTE.size();
    for(i = 0; i < r2 ; i++){
        for(k = 0; k < r1 ; k++){
            if(k==(r1-1)){
                break;
            }
            if(CURVA_COMPUESTA_CALIENTE[i][1]>CURVA_COMPUESTA_FRIA[k][1]
                    && CURVA_COMPUESTA_CALIENTE[i][1]<CURVA_COMPUESTA_FRIA[k+1][1]){
                InterpolacionFrias.resize(1);
                InterpolacionFrias[0].resize(2);
                InterpolacionFrias[0][0] = CURVA_COMPUESTA_FRIA[k][0]+
                        ((CURVA_COMPUESTA_FRIA[k+1][0]-CURVA_COMPUESTA_FRIA[k][0])
                        /(CURVA_COMPUESTA_FRIA[k+1][1]-CURVA_COMPUESTA_FRIA[k][1]))*
                        (CURVA_COMPUESTA_CALIENTE[i][1]-CURVA_COMPUESTA_FRIA[k][1]);
                InterpolacionFrias[0][1] = CURVA_COMPUESTA_CALIENTE[i][1] ;
                CURVA_COMPUESTA_FRIA = CURVA_COMPUESTA_FRIA + InterpolacionFrias;
                InterpolacionFrias.clear();
                k = 0;
                r1 = CURVA_COMPUESTA_FRIA.size();
                j++;
                break;
            }
        }
    }
    InterpolacionFrias.resize(CURVA_COMPUESTA_FRIA.size());
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        InterpolacionFrias[i].resize(2);
    }
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_FRIA.at(i).size() ; j++){
            InterpolacionFrias[i][j] = CURVA_COMPUESTA_FRIA[i][j];
        }
    }
    CURVA_COMPUESTA_FRIA.clear();
    nfils = CCFRIA.size();
    ncols = 2;
    CURVA_COMPUESTA_FRIA.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_FRIA[i].resize(ncols);
    }
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_FRIA.at(i).size() ; j++){
             CURVA_COMPUESTA_FRIA[i][j] = CCFRIA[i][j];
        }
    }
    std::sort(InterpolacionFrias.begin(),InterpolacionFrias.end());
    QVector<double> suma_inter_serfri;
    suma_inter_serfri.resize(CURVA_COMPUESTA_CALIENTE.size());
    for ( i = 0; i <  CURVA_COMPUESTA_CALIENTE.size() ; i++){
        if(CURVA_COMPUESTA_FRIA[0][1] > CURVA_COMPUESTA_CALIENTE[i][1]){
            suma_inter_serfri[i] = 1;
        }else{
            suma_inter_serfri[i] = 0;
        }
    }
    QVector<double> suma_inter_sercal;
    suma_inter_sercal.resize(CURVA_COMPUESTA_FRIA.size());
    for ( i = 0; i <  CURVA_COMPUESTA_FRIA.size() ; i++){
        if(CURVA_COMPUESTA_FRIA[i][1] > CURVA_COMPUESTA_CALIENTE[CURVA_COMPUESTA_CALIENTE.size()-1][1]){
            suma_inter_sercal[i] = 1;
        }else{
            suma_inter_sercal[i] = 0;
        }
    }
    double SUMA_inter_serfri = std::accumulate(suma_inter_serfri.begin(),suma_inter_serfri.end(),0);
    double SUMA_inter_sercal = std::accumulate(suma_inter_sercal.begin(),suma_inter_sercal.end(),0);
    QVector<QVector<double>> INTERVALOSCALIENTES,INTERVALOSFRIAS;
    nfils = SUMA_inter_sercal + InterpolacionCalientes.size()-1;
    ncols = 2;
    INTERVALOSCALIENTES.resize(nfils);
    for ( i = 0 ; i < nfils ; i++){
        INTERVALOSCALIENTES[i].resize(ncols);
    }
    j=0;
    for ( i = 0 ; i <= nfils ; i++){
        if(i == nfils){
            break;
        }
        if( i < InterpolacionCalientes.size()-1){
            INTERVALOSCALIENTES[i][0] = InterpolacionCalientes[j][0];
            INTERVALOSCALIENTES[i][1] = InterpolacionCalientes[j+1][0];
            j++;
        }else{
            INTERVALOSCALIENTES[i][0] = Calentamiento[0];
            INTERVALOSCALIENTES[i][1] = Calentamiento[1];
        }
    }
    nfils = SUMA_inter_serfri + InterpolacionFrias.size() -1;
    ncols = 2;
    INTERVALOSFRIAS.resize(nfils);
    for ( i = 0 ; i < nfils ; i++){
        INTERVALOSFRIAS[i].resize(ncols);
    }
    j=0;
    for ( i = 0 ; i < nfils ; i++){
        if(i == nfils){
            break;
        }
        if( i < SUMA_inter_serfri){
            INTERVALOSFRIAS[i][0] = Enfriamento[0];
            INTERVALOSFRIAS[i][1] = Enfriamento[1];
        }else{
            INTERVALOSFRIAS[i][0] = InterpolacionFrias[j][0];
            INTERVALOSFRIAS[i][1] = InterpolacionFrias[j+1][0];
            j++;
        }
    }
    QVector<QVector<double>> INTERVALOS_AGRUPADOS;
    nfils = INTERVALOSFRIAS.size();
    ncols = 4;
    INTERVALOS_AGRUPADOS.resize(nfils);
    MINTERVALOS_AGRUPADOS.resize(nfils);
    for ( i = 0; i < nfils ; i++){
        INTERVALOS_AGRUPADOS[i].resize(ncols);
        MINTERVALOS_AGRUPADOS[i].resize(ncols);
    }
    for ( i = 0; i < nfils ; i++){
        INTERVALOS_AGRUPADOS[i][0] = INTERVALOSCALIENTES[i][0];
        INTERVALOS_AGRUPADOS[i][1] = INTERVALOSCALIENTES[i][1];
        INTERVALOS_AGRUPADOS[i][2] = INTERVALOSFRIAS[i][0];
        INTERVALOS_AGRUPADOS[i][3] = INTERVALOSFRIAS[i][1];
        MINTERVALOS_AGRUPADOS[i][0] = INTERVALOSCALIENTES[i][0];
        MINTERVALOS_AGRUPADOS[i][1] = INTERVALOSCALIENTES[i][1];
        MINTERVALOS_AGRUPADOS[i][2] = INTERVALOSFRIAS[i][0];
        MINTERVALOS_AGRUPADOS[i][3] = INTERVALOSFRIAS[i][1];
    }
    double Delta2,Delta1;
    QVector<double> DTm;
    r1 = INTERVALOS_AGRUPADOS.size();
    DTm.resize(r1);
    MDTmed.resize(r1);
    MDelta1.resize(r1);
    MDelta2.resize(r1);
    if(CTo==1){
        for(i=0; i < r1; i++){
        Delta2= INTERVALOS_AGRUPADOS[i][3]-INTERVALOS_AGRUPADOS[i][1];
        Delta1= INTERVALOS_AGRUPADOS[i][2]-INTERVALOS_AGRUPADOS[i][0];
        DTm[i] = qFabs((Delta2-Delta1)/(log(Delta2/Delta1)));
        if(DTm[i] != DTm[i]){
            DTm[i] = (qFabs(Delta2) + qFabs(Delta1))/ 2;
        }
        MDelta1[i] = Delta1;
        MDelta2[i] = Delta2;
        MDTmed[i] = DTm[i];
        }
    }else if(CCo==1){
        for(i=0; i < r1; i++){
        Delta2= INTERVALOS_AGRUPADOS[i][3]-INTERVALOS_AGRUPADOS[i][1];
        Delta1= INTERVALOS_AGRUPADOS[i][0]-INTERVALOS_AGRUPADOS[i][2];
        DTm[i] = qFabs((Delta2-Delta1)/(log(Delta2/Delta1)));
        if(DTm[i] != DTm[i]){
            DTm[i] = (qFabs(Delta2) + qFabs(Delta1))/ 2;
        }
        MDelta1[i] = Delta1;
        MDelta2[i] = Delta2;
        MDTmed[i] = DTm[i];
        }
    }
    QVector<QVector<double>> Calcal;
    r1 = INTERVALOSCALIENTES.size(); //CHECAR
    r2 = VectorCalientesMATRIZ.size();
    Calcal.resize(r1);
    for( i = 0; i < r1 ; i++){
        Calcal[i].resize(r2);
    }
    for( i = 0; i < r1 ; i++){
        for ( j = 0; j < r2; j++){
            if(INTERVALOSCALIENTES[i][0] < VectorCalientesMATRIZ[j][0]  && INTERVALOSCALIENTES[i][0] > VectorCalientesMATRIZ[j][1]-1){
                Calcal[i][j] = (VectorCalientesMATRIZ[j][2]*(INTERVALOSCALIENTES[i][1]-INTERVALOSCALIENTES[i][0]))
                        /VectorCalientesMATRIZ[j][3];
            }else{
                Calcal[i][j] = 0;
            }
        }
    }
    QVector<QVector<double>> Calfri;
    //r1 = INTERVALOSFRIAS.size();
    r1 = INTERVALOSFRIAS.size();  //CHECAR
    r2 = VectorFriasMATRIZ.size();
    Calfri.resize(r1);
    for( i = 0 ; i < r1; i++){
        Calfri[i].resize(r2);
    }
    for( i = 0; i < r1 ; i++){
        for ( j = 0; j < r2; j++){
            if(INTERVALOSFRIAS[i][0] < VectorFriasMATRIZ[j][1] && INTERVALOSFRIAS[i][0] > VectorFriasMATRIZ[j][0]-1){
                Calfri[i][j] = (VectorFriasMATRIZ[j][2]*(INTERVALOSFRIAS[i][1]-INTERVALOSFRIAS[i][0]))
                        /VectorFriasMATRIZ[j][3];
            }else{
                Calfri[i][j] = 0;
            }
        }
    }
    QVector<double> Entalpia,SUMCalcal,SUMCalfri;
    nfils = Calcal.size();
    SUMCalcal.resize(nfils);
    for( i = 0; i < nfils ; i++){
        SUMCalcal[i] = 0;
        for ( j = 0; j < Calcal[0].size() ; j++)
            SUMCalcal[i] = SUMCalcal[i] + Calcal[i][j];
    }
    nfils = Calfri.size();
    SUMCalfri.resize(nfils);
    for( i = 0; i < nfils ; i++){
        SUMCalfri[i] = 0;
        for ( j = 0; j < Calfri[0].size() ; j++)
            SUMCalfri[i] = SUMCalfri[i] + Calfri[i][j];
    }
    Entalpia.resize(nfils);
    for( i = 0; i < nfils ; i++){
         Entalpia[i] = SUMCalcal[i] + SUMCalfri[i];
    }
    int r1CAL = SUMA_inter_sercal;
    int r2FRI = SUMA_inter_serfri;
    for ( i = 0 ; i < r2FRI ; i++){
        Entalpia[i] = Entalpia[i] + Intervalos_de_Entalpia[i]/Enfriamento[2];
    }
    for ( i = Entalpia.size()-r1CAL; i < Entalpia.size(); i++){
        Entalpia[i] = Entalpia[i] + Intervalos_de_Entalpia[i]/Calentamiento[2];
    }
    //CALCULO DE AREAS;
    QVector<double> Areas;
    Areas.resize(Entalpia.size());
    MVectorAreas.resize(Entalpia.size());
    MVectorEntalpia.resize(Entalpia.size());
    for ( i = 0 ; i < Entalpia.size(); i++){
        Areas[i] = Entalpia[i] / DTm[i];
        MVectorEntalpia[i] = Entalpia[i];
        MVectorAreas[i] = Areas[i];
    }
    MAREAS = std::accumulate(Areas.begin(),Areas.end(),0);
}

double Plot_Dtmin_vs_Areas::getAREAS()
{
    return MAREAS;
}

QVector<double> Plot_Dtmin_vs_Areas::getDTmed()
{
    return MDTmed;
}

QVector<double> Plot_Dtmin_vs_Areas::getDelta2()
{
    return MDelta2;
}

QVector<double> Plot_Dtmin_vs_Areas::getDelta1()
{
    return MDelta1;
}

QVector<double> Plot_Dtmin_vs_Areas::getVectorAreas()
{
    return MVectorAreas;
}

QVector<double> Plot_Dtmin_vs_Areas::getVectorEntalpia()
{
    return MVectorEntalpia;
}

QVector<QVector<double> > Plot_Dtmin_vs_Areas::getINTERVALOS_AGRUPADOS()
{
    return MINTERVALOS_AGRUPADOS;
}


Plot_grancurvacompuesta::Plot_grancurvacompuesta(QVector<double> Tsupply, QVector<double> Ttarget,
                                                 QVector<double> Cp, double DTmin)
{
    QVector<double> VectorCalientes, VectorCalientesCp;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas;
    int i,j,n,nfils,ncols,Pares,contador,k;
    n = Tsupply.size();
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    //qDebug() << VectorCalientes << VectorCalientesCp << VectorFrias << VectorFriasCp;
    QVector < QVector <double> > VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VCC,VFC;
    Pares = VectorCalientes.size();
    nfils = Pares/2;
    ncols = 2;
    VCC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VCC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VCC[i][0] = VectorCalientes[k];
        k++;
        VCC[i][1] = VectorCalientes[k];
        k++;
    }
    Pares = VectorFrias.size();
    nfils = Pares/2;
    ncols = 2;
    VFC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VFC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VFC[i][0] = VectorFrias[k];
        k++;
        VFC[i][1] = VectorFrias[k];
        k++;
    }
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    QVector<double> VCCurvasT = VCCurvas;
    QVector<double> VFCurvasT = VFCurvas;
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size();//2;
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l=0;
    int m =VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0]-1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-1 )
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size();//2;
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    //MODIFICADO
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val1;
    n = CpcurvaCold.size();
    for(i=0; i < n ; i++){
        Val1 << RangosFrias[i][2] * CpcurvaCold[i];
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    // HASTA AQUI ES PARA CURVAS COMPUESTAS
    QVector<QVector<double>> VectorCalientesTmin;
    VectorCalientesTmin.resize(VCCurvasT.size());
    nfils = VCCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i].resize(ncols);
    }
    k = VCCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i][0] = VCCurvasT[k]-DTmin;
        VectorCalientesTmin[i][1] = VCCurvasT[k];
        --k;
    }
    QVector<QVector<double>> VectorFriasTmin;
    VectorFriasTmin.resize(VFCurvasT.size());
    nfils = VFCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i].resize(ncols);
    }
    k = VFCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i][0] = VFCurvasT[k];
        VectorFriasTmin[i][1] = VFCurvasT[k]+DTmin;
        --k;
    }
    QVector<QVector<double>> VectorCorrientesTotal;
    nfils = VCCurvasT.size() + VFCurvasT.size();
    ncols = 2;
    VectorCorrientesTotal.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotal[i].resize(ncols);
    }
    j=0;
    k=0;
    for(i = 0; i < nfils ; i++){
        if(i < VectorCalientesTmin.size()){
            VectorCorrientesTotal[i][0] = VectorCalientesTmin[j][0];
            VectorCorrientesTotal[i][1] = VectorCalientesTmin[j][1];
            ++j;
        }else{
            VectorCorrientesTotal[i][0] = VectorFriasTmin[k][0];
            VectorCorrientesTotal[i][1] = VectorFriasTmin[k][1];
            ++k;
        }
    }
    std::sort(VectorCorrientesTotal.begin(),VectorCorrientesTotal.end());
    QVector<QVector<double>> VectorCorrientesTotalOrdenar;
    nfils = VectorCorrientesTotal.size();
    ncols = 2;
    VectorCorrientesTotalOrdenar.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i].resize(ncols);
    }
    k = VectorCorrientesTotal.size()-1;
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i][0] = VectorCorrientesTotal[k][0];
        VectorCorrientesTotalOrdenar[i][1] = VectorCorrientesTotal[k][1];
        --k;
    }
    VectorCorrientesTotal = VectorCorrientesTotalOrdenar;
    int r3 = VectorCorrientesTotal.size();
    int r4 = VCC.size();
    //FORMAR LA MATRIZ PARA SUMCPCAL
    QVector<QVector<double>> sumCpCalTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VCC.size();//2;
    sumCpCalTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpCalTab[i].resize(ncols);
    }
    l = 0;
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r4; ++j){
            if(VectorCorrientesTotal[i][1] <= VCC[j][0]-1 && VectorCorrientesTotal[i][1] >= VCC[j][1] ){
                sumCpCalTab[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCalTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<QVector<double>> sumCpFriTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VFC.size();//2;
    sumCpFriTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpFriTab[i].resize(ncols);
    }
    l = 0;
    int r5 = VFC.size();
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r5; ++j){
            if(VectorCorrientesTotal[i][1] <= VFC[j][1]-1 && VectorCorrientesTotal[i][1] >= VFC[j][0] ){
                sumCpFriTab[l][k] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFriTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<double> SumatoriaCP,SumatoriaCPCAL,SumatoriaCPFRI;
    nfils = VectorCorrientesTotal.size()-1;
    SumatoriaCP.resize(nfils);
    SumatoriaCPCAL.resize(nfils);
    SumatoriaCPFRI.resize(nfils);
    for(i = 0; i < nfils ; i++){
        SumatoriaCPCAL[i] = 0;
        for(j = 0 ; j < sumCpCalTab[0].size();j++){
            SumatoriaCPCAL[i] = SumatoriaCPCAL[i] + sumCpCalTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCPFRI[i] = 0;
        for(j = 0 ; j < sumCpFriTab[0].size(); j++){
            SumatoriaCPFRI[i] = SumatoriaCPFRI[i] + sumCpFriTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCP[i] = SumatoriaCPFRI[i] - SumatoriaCPCAL[i] ;
    }
    QVector<double> DifTemperaturas;
    nfils = VectorCorrientesTotal.size()-1;
    DifTemperaturas.resize(nfils);
    j = 1;
    for(i = 0; i < VectorCorrientesTotal.size() ; ++i){
        if (j < VectorCorrientesTotal.size()){
            DifTemperaturas[i] = VectorCorrientesTotal[i][1] - VectorCorrientesTotal[j][1];
            ++j;
        }else if(j == VectorCorrientesTotal.size()){
            break;
        }
    }
    QVector<double> Deficit;
    nfils = DifTemperaturas.size();
    Deficit.resize(DifTemperaturas.size());
    for( i = 0 ; i < nfils; ++i){
        Deficit[i] = DifTemperaturas[i] * SumatoriaCP[i];
    }
    int Ldef = Deficit.size();
    QVector<QVector<double>> VecHeatFlow;
    QVector<double> VAL1,VAL2;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecHeatFlow.resize(nfils);
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecHeatFlow[i].resize(ncols);
    }
    VAL1[0]=0;
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecHeatFlow[i][0] = VAL1[i]; // Val1
        VecHeatFlow[i][1] = VAL2[i]; // Val2
    }
    double min = *std::min_element(VAL2.begin(),VAL2.end());
    VAL1.clear();
    VAL2.clear();
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    QVector<QVector<double>> VecAdjHeatFlow;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecAdjHeatFlow.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecAdjHeatFlow[i].resize(ncols);
    }
    VAL1[0]= qFabs(min);
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecAdjHeatFlow[i][0] = VAL1[i]; // Val1
        VecAdjHeatFlow[i][1] = VAL2[i]; // Val2
    }
    // PUNTOS DE LA CURVAS COMPUESTAS
    QVector<double> TGC;
    nfils = VectorCorrientesTotal.size();
    TGC.resize(nfils);
    for (i = 0; i < nfils; i++){
        TGC[i] = (VectorCorrientesTotal[i][0] + VectorCorrientesTotal[i][1])/2;
    }
    QVector<double> EGC;
    EGC.resize(nfils);
    j=0;
    for (i = 0; i < nfils; i++){
        if(i == 0){
            EGC[i] = VecAdjHeatFlow[0][0];
        }else{
            EGC[i] = VecAdjHeatFlow[j][1];
            j++;
        }
    }
    MGCTEMPERATURAS.resize(TGC.size());
    MGCENTALPIA.resize(EGC.size());
    for(i=0;i < TGC.size();i++){
        MGCTEMPERATURAS[i] = TGC[i];
        MGCENTALPIA[i] = EGC[i];
    }
}

QVector<double> Plot_grancurvacompuesta::getGCTEMPERATURAS()
{
    return MGCTEMPERATURAS;
}

QVector<double> Plot_grancurvacompuesta::getGCENTALPIA()
{
    return MGCENTALPIA;
}

Plot_curvascompuestas_diversa::Plot_curvascompuestas_diversa(QVector<double> Tsupply, QVector<double> Ttarget,
                                                             QVector<double> Cp, QVector<double> h, double DTmin,
                                                             double K,float punto1,float punto2)
{
    int i,j,n,nfils,ncols,Pares,contador,k;
    Plot_curvascompuestasajustadas plot1(Tsupply,Ttarget,Cp,DTmin);
    QVector<QVector<double>> VADHF = plot1.getVecAdjHeatFlow();
    float UTILIDADCALENTAMIENTO = VADHF[0][0];
    float UTILIDADFRIA = VADHF[VADHF.size()-1][1];
    double ERROR;
    float error1,error2,error3,error = 100.00, errorPasado = 100.00,tolerancia = 0.000001;
    QVector<float> ValoresdeK,ValoresdeError;
    ValoresdeK.resize(31);
    ValoresdeError.resize(31);
    i = 0;
    int iterador = 0;
    int min_pos;
    while(error >= tolerancia ){//while(error >= tolerancia ){
        float incremento = (punto2-punto1)/30.0;
        ValoresdeK[0] = double (punto1);
        for(int i = 1; i <= 30; i++){
            ValoresdeK[i] = float (punto1 + incremento);
            punto1 = punto1 + incremento;
        }
        for(int i = 0; i < ValoresdeK.size(); i++){
            K = double (ValoresdeK[i]);
            FindKvalue Ki(Tsupply,Ttarget,Cp,h,K,DTmin);
            float KiUC = Ki.getUC();
            float KiUF = Ki.getUF();
            error1 = qFabs( ((UTILIDADCALENTAMIENTO - KiUC)/KiUC));
            error2 = qFabs( ((UTILIDADFRIA - KiUF )/KiUF ));
            error3 = error1 + error2;
            ValoresdeError[i] = float (error3);
        }
        min_pos = std::distance(ValoresdeError.begin(),std::min_element(ValoresdeError.begin(),ValoresdeError.end()));
        ERROR = *std::min_element(ValoresdeError.begin(),ValoresdeError.end()); //double
        error = float (ERROR);
        if(error == errorPasado){
            break;
        }
        errorPasado = error;
        if(error > tolerancia){
            punto1 = ValoresdeK[min_pos]-incremento;
            punto2 = ValoresdeK[min_pos]+incremento;
            iterador = iterador +1;
        }
    }
    MK = K;
    K = ValoresdeK[min_pos];
    QVector<double> DeltasTi;
    n = Tsupply.size();
    DeltasTi.resize(n);
    QVector<double> VectorCalientes, VectorCalientesCp,VCC;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas,VFC;
    n = Tsupply.size();
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            DeltasTi[i] = K / h[i];
            Tsupply[i] = Tsupply[i] + (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] + (DTmin/2 - DeltasTi[i]);
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            DeltasTi[i] = K / h[i];
            Tsupply[i] = Tsupply[i] - (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] - (DTmin/2 - DeltasTi[i]);
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    QVector < QVector <double> > VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(int j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    VCC = VectorCalientes;
    VFC = VectorFrias;
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    //qDebug() << VectorCalientes << VectorFrias;
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    VectorCalientes = VectorCalientesNuevo;
    VectorFrias = VectorFriasNuevo;
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size();//n; //corregido
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l=0;
    int m =VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0]-.1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-.1 )
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size();//2;
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-.1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-.1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    // sumas de vector para sumCpFri y sumCpCal
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val1;
    n = CpcurvaCold.size();
    for(i=0; i < n ; i++){
        Val1 << RangosFrias[i][2] * CpcurvaCold[i];
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    QVector<double> PuntosCurvasC,PuntosCurvasF;
    n = PUNTOSCC.size();
    PuntosCurvasC.resize(PUNTOSCC.size()+1);
    PuntosCurvasC[0] = 0;
    for(i = 0 ; i < n; i++){
        PuntosCurvasC[i+1] = PuntosCurvasC[i] + PUNTOSCC[i][3];
    }
    n = PUNTOSCF.size();
    PuntosCurvasF.resize(PUNTOSCF.size()+1);
    PuntosCurvasF[0] = 0;
    for(i = 0; i < n; i++){
        PuntosCurvasF[i+1] = PuntosCurvasF[i] + PUNTOSCF[i][3];
    }
    MCCENTALPIA.resize(PuntosCurvasC.size());
    MCCTEMPERATURAS.resize(VCCurvas.size());
    for ( int i = 0 ; i < PuntosCurvasC.size() ; i++){
        MCCENTALPIA[i] = PuntosCurvasC[i];
        MCCTEMPERATURAS[i] = VCCurvas[i] - DTmin/2;
    }
    MCFENTALPIA.resize(PuntosCurvasF.size());
    MCFTEMPERATURAS.resize(VFCurvas.size());
    for ( int i = 0 ; i < PuntosCurvasF.size() ; i++){
        MCFENTALPIA[i] = PuntosCurvasF[i];
        MCFTEMPERATURAS[i] = VFCurvas[i] + DTmin/2;
    }
}

QVector<double> Plot_curvascompuestas_diversa::getCCENTALPIA()
{
    return MCCENTALPIA;
}

QVector<double> Plot_curvascompuestas_diversa::getCCTEMPERATURAS()
{
    return MCCTEMPERATURAS;
}

QVector<double> Plot_curvascompuestas_diversa::getCFENTALPIA()
{
    return MCFENTALPIA;
}

QVector<double> Plot_curvascompuestas_diversa::getCFTEMPERATURAS()
{
    return MCFTEMPERATURAS;
}

double Plot_curvascompuestas_diversa::getK()
{
    return MK;
}

Plot_CCAJUSTADA_DIVERSA::Plot_CCAJUSTADA_DIVERSA(QVector<double> Tsupply, QVector<double> Ttarget,
                                                 QVector<double> Cp, QVector<double> h,
                                                 double DTmin, double K, float punto1, float punto2)
{
    int i,j,n,nfils,ncols,Pares,contador,k;
    Plot_curvascompuestasajustadas plot1(Tsupply,Ttarget,Cp,DTmin);
    QVector<QVector<double>> VADHF = plot1.getVecAdjHeatFlow();
    float UTILIDADCALENTAMIENTO = VADHF[0][0];
    float UTILIDADFRIA = VADHF[VADHF.size()-1][1];
    double ERROR;
    float error1,error2,error3,error = 100.00, errorPasado = 100.00,tolerancia = 0.000001;
    QVector<float> ValoresdeK,ValoresdeError;
    ValoresdeK.resize(31);
    ValoresdeError.resize(31);
    i = 0;
    int iterador = 0;
    int min_pos;
    while(error >= tolerancia ){//while(error >= tolerancia ){
        float incremento = (punto2-punto1)/30.0;
        ValoresdeK[0] = double (punto1);
        for(int i = 1; i <= 30; i++){
            ValoresdeK[i] = float (punto1 + incremento);
            punto1 = punto1 + incremento;
        }
        for(int i = 0; i < ValoresdeK.size(); i++){
            K = double (ValoresdeK[i]);
            FindKvalue Ki(Tsupply,Ttarget,Cp,h,K,DTmin);
            float KiUC = Ki.getUC();
            float KiUF = Ki.getUF();
            error1 = qFabs( ((UTILIDADCALENTAMIENTO - KiUC)/KiUC));
            error2 = qFabs( ((UTILIDADFRIA - KiUF )/KiUF ));
            error3 = error1 + error2;
            ValoresdeError[i] = float (error3);
        }
        min_pos = std::distance(ValoresdeError.begin(),std::min_element(ValoresdeError.begin(),ValoresdeError.end()));
        ERROR = *std::min_element(ValoresdeError.begin(),ValoresdeError.end()); //double
        error = float (ERROR);
        if(error == errorPasado){
            break;
        }
        errorPasado = error;
        if(error > tolerancia){
            punto1 = ValoresdeK[min_pos]-incremento;
            punto2 = ValoresdeK[min_pos]+incremento;
            iterador = iterador +1;
        }
    }
    MK = K;
    K = ValoresdeK[min_pos];
    QVector<double> DeltasTi;
    n = Tsupply.size();
    DeltasTi.resize(n);
    for (i = 0; i < n; i++){
        DeltasTi[i] = K / h[i];
    }
    QVector<double> VectorCalientes, VectorCalientesCp;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas;
    n = Tsupply.size();
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            DeltasTi[i] =(K / (h[i]/1000));
            Tsupply[i] = Tsupply[i] + (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] + (DTmin/2 - DeltasTi[i]);
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            DeltasTi[i] =(K / (h[i]/1000));
            Tsupply[i] = Tsupply[i] - (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] - (DTmin/2 - DeltasTi[i]);
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    QVector < QVector <double> > VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VCC,VFC;
    Pares = VectorCalientes.size();
    nfils = Pares/2;
    ncols = 2;
    VCC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VCC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VCC[i][0] = VectorCalientes[k];
        k++;
        VCC[i][1] = VectorCalientes[k];
        k++;
    }
    Pares = VectorFrias.size();
    nfils = Pares/2;
    ncols = 2;
    VFC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VFC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VFC[i][0] = VectorFrias[k];
        k++;
        VFC[i][1] = VectorFrias[k];
        k++;
    }
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    QVector<double> VCCurvasT = VCCurvas;
    QVector<double> VFCurvasT = VFCurvas;
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size();//2;
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l=0;
    int m =VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0]-.1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-.1 )
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size();//2;
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-.1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-.1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    // HASTA AQUI ES PARA CURVAS COMPUESTAS
    QVector<QVector<double>> VectorCalientesTmin;
    VectorCalientesTmin.resize(VCCurvasT.size());
    nfils = VCCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i].resize(ncols);
    }
    k = VCCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i][0] = VCCurvasT[k]-DTmin;
        VectorCalientesTmin[i][1] = VCCurvasT[k];
        --k;
    }
    QVector<QVector<double>> VectorFriasTmin;
    VectorFriasTmin.resize(VFCurvasT.size());
    nfils = VFCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i].resize(ncols);
    }
    k = VFCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i][0] = VFCurvasT[k];
        VectorFriasTmin[i][1] = VFCurvasT[k]+DTmin;
        --k;
    }
    QVector<QVector<double>> VectorCorrientesTotal;
    nfils = VCCurvasT.size() + VFCurvasT.size();
    ncols = 2;
    VectorCorrientesTotal.resize(nfils);
    MVectorCorrientesTotal.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotal[i].resize(ncols);
        MVectorCorrientesTotal[i].resize(ncols);
    }
    j=0;
    k=0;
    for(i = 0; i < nfils ; i++){
        if(i < VectorCalientesTmin.size()){
            VectorCorrientesTotal[i][0] = VectorCalientesTmin[j][0];
            VectorCorrientesTotal[i][1] = VectorCalientesTmin[j][1];
            ++j;
        }else{
            VectorCorrientesTotal[i][0] = VectorFriasTmin[k][0];
            VectorCorrientesTotal[i][1] = VectorFriasTmin[k][1];
            ++k;
        }
    }
    std::sort(VectorCorrientesTotal.begin(),VectorCorrientesTotal.end());
    QVector<QVector<double>> VectorCorrientesTotalOrdenar;
    nfils = VectorCorrientesTotal.size();
    ncols = 2;
    VectorCorrientesTotalOrdenar.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i].resize(ncols);
    }
    k = VectorCorrientesTotal.size()-1;
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i][0] = VectorCorrientesTotal[k][0];
        VectorCorrientesTotalOrdenar[i][1] = VectorCorrientesTotal[k][1];
        --k;
    }
    VectorCorrientesTotal = VectorCorrientesTotalOrdenar;
    MVectorCorrientesTotal = VectorCorrientesTotal;
    int r3 = VectorCorrientesTotal.size();
    int r4 = VCC.size();
    //FORMAR LA MATRIZ PARA SUMCPCAL
    QVector<QVector<double>> sumCpCalTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VCC.size();//2;
    sumCpCalTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpCalTab[i].resize(ncols);
    }
    l = 0;
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r4; ++j){
            if(VectorCorrientesTotal[i][1] <= VCC[j][0]-.1 && VectorCorrientesTotal[i][1] >= VCC[j][1]-.1 ){
                sumCpCalTab[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCalTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<QVector<double>> sumCpFriTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VFC.size();//2;
    sumCpFriTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpFriTab[i].resize(ncols);
    }
    l = 0;
    int r5 = VFC.size();
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r5; ++j){
            if(VectorCorrientesTotal[i][0] <= VFC[j][1]-.1 && VectorCorrientesTotal[i][0] >= VFC[j][0]-.1 ){
                sumCpFriTab[l][k] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFriTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<double> SumatoriaCP,SumatoriaCPCAL,SumatoriaCPFRI;
    nfils = VectorCorrientesTotal.size()-1;
    SumatoriaCP.resize(nfils);
    SumatoriaCPCAL.resize(nfils);
    SumatoriaCPFRI.resize(nfils);
    for(i = 0; i < nfils ; i++){
        SumatoriaCPCAL[i] = 0;
        for(j = 0 ; j < sumCpCalTab[0].size();j++){
            SumatoriaCPCAL[i] = SumatoriaCPCAL[i] + sumCpCalTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCPFRI[i] = 0;
        for(j = 0 ; j < sumCpFriTab[0].size(); j++){
            SumatoriaCPFRI[i] = SumatoriaCPFRI[i] + sumCpFriTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCP[i] = SumatoriaCPFRI[i] - SumatoriaCPCAL[i] ;
    }
    QVector<double> DifTemperaturas;
    nfils = VectorCorrientesTotal.size()-1;
    DifTemperaturas.resize(nfils);
    j = 1;
    for(i = 0; i < VectorCorrientesTotal.size() ; ++i){
        if (j < VectorCorrientesTotal.size()){
            DifTemperaturas[i] = VectorCorrientesTotal[i][1] - VectorCorrientesTotal[j][1];
            ++j;
        }else if(j == VectorCorrientesTotal.size()){
            break;
        }
    }
    QVector<double> Deficit;
    nfils = DifTemperaturas.size();
    Deficit.resize(DifTemperaturas.size());
    MDeficit.resize(DifTemperaturas.size());
    for( i = 0 ; i < nfils; ++i){
        Deficit[i] = DifTemperaturas[i] * SumatoriaCP[i];
        MDeficit[i] = Deficit[i];
    }
    int Ldef = Deficit.size();
    QVector<QVector<double>> VecHeatFlow;
    QVector<double> VAL1,VAL2;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecHeatFlow.resize(nfils);
    MVecHeatFlow.resize(nfils);
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecHeatFlow[i].resize(ncols);
        MVecHeatFlow[i].resize(ncols);
    }
    VAL1[0]=0;
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecHeatFlow[i][0] = VAL1[i]; // Val1
        VecHeatFlow[i][1] = VAL2[i]; // Val2
        MVecHeatFlow[i][0] = VecHeatFlow[i][0];
        MVecHeatFlow[i][1] = VecHeatFlow[i][1];
    }
    double min = *std::min_element(VAL2.begin(),VAL2.end());
    VAL1.clear();
    VAL2.clear();
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    QVector<QVector<double>> VecAdjHeatFlow;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecAdjHeatFlow.resize(nfils);
    MVecAdjHeatFlow.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecAdjHeatFlow[i].resize(ncols);
        MVecAdjHeatFlow[i].resize(ncols);
    }
    VAL1[0]= qFabs(min);
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecAdjHeatFlow[i][0] = VAL1[i]; // Val1
        VecAdjHeatFlow[i][1] = VAL2[i]; // Val2
        MVecAdjHeatFlow[i][0] = VecAdjHeatFlow[i][0];
        MVecAdjHeatFlow[i][1] = VecAdjHeatFlow[i][1];
    }
    // PUNTOS DE LA CURVAS COMPUESTAS
    QVector<double> PuntosCurvasC,PuntosCurvasF;
    n = PUNTOSCC.size();
    PuntosCurvasC.resize(PUNTOSCC.size()+1);
    PuntosCurvasC[0] = 0;
    for(i = 0 ; i < n; i++){
        PuntosCurvasC[i+1] = PuntosCurvasC[i] + PUNTOSCC[i][3];
    }
    n = PUNTOSCF.size();
    PuntosCurvasF.resize(PUNTOSCF.size()+1);
    //double max = *std::max_element(VAL2.begin(),VAL2.end());
    double max = VecAdjHeatFlow[VecAdjHeatFlow.size()-1][1];
    PuntosCurvasF[0] = max ;
    for(i = 0; i < n; i++){
        PuntosCurvasF[i+1] = PuntosCurvasF[i] + PUNTOSCF[i][3];
    }
    MCCENTALPIA.resize(PuntosCurvasC.size());
    MCCTEMPERATURAS.resize(VCCurvas.size());
    for ( int i = 0 ; i < PuntosCurvasC.size() ; i++){
        MCCENTALPIA[i] = PuntosCurvasC[i];
        MCCTEMPERATURAS[i] = VCCurvas[i] - DTmin/2;
    }
    MCFENTALPIAAJUSTADA.resize(PuntosCurvasF.size());
    MCFTEMPERATURASAJUSTADA.resize(VFCurvas.size());
    for ( int i = 0 ; i < PuntosCurvasF.size() ; i++){
        MCFENTALPIAAJUSTADA[i] = PuntosCurvasF[i];
        MCFTEMPERATURASAJUSTADA[i] = VFCurvas[i] + DTmin/2;
    }
}

QVector<double> Plot_CCAJUSTADA_DIVERSA::getCCENTALPIA()
{
    return MCCENTALPIA;
}

QVector<double> Plot_CCAJUSTADA_DIVERSA::getCCTEMPERATURAS()
{
    return MCCTEMPERATURAS;
}

QVector<double> Plot_CCAJUSTADA_DIVERSA::getCFENTALPIAAJUSTADAS()
{
    return MCFENTALPIAAJUSTADA;
}

QVector<double> Plot_CCAJUSTADA_DIVERSA::getCFTEMPERATURASAJUSTADAS()
{
    return MCFTEMPERATURASAJUSTADA;
}

QVector<QVector<double> > Plot_CCAJUSTADA_DIVERSA::getVectorCorrientesTotal()
{
    return MVectorCorrientesTotal;
}

QVector<QVector<double> > Plot_CCAJUSTADA_DIVERSA::getVecHeatFlow()
{
    return MVecHeatFlow;
}

QVector<QVector<double> > Plot_CCAJUSTADA_DIVERSA::getVecAdjHeatFlow()
{
    return MVecAdjHeatFlow;
}

QVector<double> Plot_CCAJUSTADA_DIVERSA::getDeficit()
{
    return MDeficit;
}

double Plot_CCAJUSTADA_DIVERSA::getK()
{
    return MK;
}

PlotGCC_DIVERSA::PlotGCC_DIVERSA(QVector<double> Tsupply, QVector<double> Ttarget, QVector<double> Cp,
                                 QVector<double> h, double DTmin, double K, float punto1, float punto2)
{
    QVector<double> VectorCalientes, VectorCalientesCp;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas;
    QVector<double> DeltasTi;
    Plot_curvascompuestasajustadas plot1(Tsupply,Ttarget,Cp,DTmin);
    QVector<QVector<double>> VADHF = plot1.getVecAdjHeatFlow();
    float UTILIDADCALENTAMIENTO = VADHF[0][0];
    float UTILIDADFRIA = VADHF[VADHF.size()-1][1];
    double ERROR;
    float error1,error2,error3,error = 100.00, errorPasado = 100.00,tolerancia = 0.000001;
    QVector<float> ValoresdeK,ValoresdeError;
    ValoresdeK.resize(31);
    ValoresdeError.resize(31);
    int iterador = 0;
    int min_pos;
    while(error >= tolerancia ){//while(error >= tolerancia ){
        float incremento = (punto2-punto1)/30.0;
        ValoresdeK[0] = double (punto1);
        for(int i = 1; i <= 30; i++){
            ValoresdeK[i] = float (punto1 + incremento);
            punto1 = punto1 + incremento;
        }
        for(int i = 0; i < ValoresdeK.size(); i++){
            K = double (ValoresdeK[i]);
            FindKvalue Ki(Tsupply,Ttarget,Cp,h,K,DTmin);
            float KiUC = Ki.getUC();
            float KiUF = Ki.getUF();
            error1 = qFabs( ((UTILIDADCALENTAMIENTO - KiUC)/KiUC));
            error2 = qFabs( ((UTILIDADFRIA - KiUF )/KiUF ));
            error3 = error1 + error2;
            ValoresdeError[i] = float (error3);
        }
        min_pos = std::distance(ValoresdeError.begin(),std::min_element(ValoresdeError.begin(),ValoresdeError.end()));
        ERROR = *std::min_element(ValoresdeError.begin(),ValoresdeError.end()); //double
        error = float (ERROR);
        if(error == errorPasado){
            break;
        }
        errorPasado = error;
        //qDebug() <<  "error" << error  << "Valor de K" << ValoresdeK[min_pos] << iterador << min_pos;
        if(error > tolerancia){
            punto1 = ValoresdeK[min_pos]-incremento;
            punto2 = ValoresdeK[min_pos]+incremento;
            iterador = iterador +1;
        }
    }
    MK = ValoresdeK[min_pos];
    K = ValoresdeK[min_pos];

    int i,j,n,nfils,ncols,Pares,contador,k;
    n = Tsupply.size();
    DeltasTi.resize(n);
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            DeltasTi[i] = K / h[i];
            Tsupply[i] = Tsupply[i] + (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] + (DTmin/2 - DeltasTi[i]);
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            DeltasTi[i] = K / h[i];
            Tsupply[i] = Tsupply[i] - (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] - (DTmin/2 - DeltasTi[i]);
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] ;//<< h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    QVector < QVector <double> > VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/3;
    ncols = 3;
    VectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VCC,VFC;
    Pares = VectorCalientes.size();
    nfils = Pares/2;
    ncols = 2;
    VCC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VCC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VCC[i][0] = VectorCalientes[k];
        k++;
        VCC[i][1] = VectorCalientes[k];
        k++;
    }
    Pares = VectorFrias.size();
    nfils = Pares/2;
    ncols = 2;
    VFC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VFC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VFC[i][0] = VectorFrias[k];
        k++;
        VFC[i][1] = VectorFrias[k];
        k++;
    }
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    QVector<double> VCCurvasT = VCCurvas;
    QVector<double> VFCurvasT = VFCurvas;
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size();//2;
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l=0;
    int m =VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0]-.1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-.1 )
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size();//2;
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-.1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-.1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    //MODIFICADO
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val1;
    n = CpcurvaCold.size();
    for(i=0; i < n ; i++){
        Val1 << RangosFrias[i][2] * CpcurvaCold[i];
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    // HASTA AQUI ES PARA CURVAS COMPUESTAS
    QVector<QVector<double>> VectorCalientesTmin;
    VectorCalientesTmin.resize(VCCurvasT.size());
    nfils = VCCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i].resize(ncols);
    }
    k = VCCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i][0] = VCCurvasT[k]-DTmin;
        VectorCalientesTmin[i][1] = VCCurvasT[k];
        --k;
    }
    QVector<QVector<double>> VectorFriasTmin;
    VectorFriasTmin.resize(VFCurvasT.size());
    nfils = VFCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i].resize(ncols);
    }
    k = VFCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i][0] = VFCurvasT[k];
        VectorFriasTmin[i][1] = VFCurvasT[k]+DTmin;
        --k;
    }
    QVector<QVector<double>> VectorCorrientesTotal;
    nfils = VCCurvasT.size() + VFCurvasT.size();
    ncols = 2;
    VectorCorrientesTotal.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotal[i].resize(ncols);
    }
    j=0;
    k=0;
    for(i = 0; i < nfils ; i++){
        if(i < VectorCalientesTmin.size()){
            VectorCorrientesTotal[i][0] = VectorCalientesTmin[j][0];
            VectorCorrientesTotal[i][1] = VectorCalientesTmin[j][1];
            ++j;
        }else{
            VectorCorrientesTotal[i][0] = VectorFriasTmin[k][0];
            VectorCorrientesTotal[i][1] = VectorFriasTmin[k][1];
            ++k;
        }
    }
    std::sort(VectorCorrientesTotal.begin(),VectorCorrientesTotal.end());
    QVector<QVector<double>> VectorCorrientesTotalOrdenar;
    nfils = VectorCorrientesTotal.size();
    ncols = 2;
    VectorCorrientesTotalOrdenar.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i].resize(ncols);
    }
    k = VectorCorrientesTotal.size()-1;
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i][0] = VectorCorrientesTotal[k][0];
        VectorCorrientesTotalOrdenar[i][1] = VectorCorrientesTotal[k][1];
        --k;
    }
    VectorCorrientesTotal = VectorCorrientesTotalOrdenar;
    int r3 = VectorCorrientesTotal.size();
    int r4 = VCC.size();
    //FORMAR LA MATRIZ PARA SUMCPCAL
    QVector<QVector<double>> sumCpCalTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VCC.size();//2;
    sumCpCalTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpCalTab[i].resize(ncols);
    }
    l = 0;
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r4; ++j){
            if(VectorCorrientesTotal[i][1] <= VCC[j][0]-.1 && VectorCorrientesTotal[i][1] >= VCC[j][1]-.1 ){
                sumCpCalTab[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCalTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<QVector<double>> sumCpFriTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VFC.size();//2;
    sumCpFriTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpFriTab[i].resize(ncols);
    }
    l = 0;
    int r5 = VFC.size();
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r5; ++j){
            if(VectorCorrientesTotal[i][0] <= VFC[j][1]-.1 && VectorCorrientesTotal[i][0] >= VFC[j][0]-1){
                sumCpFriTab[l][k] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFriTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<double> SumatoriaCP,SumatoriaCPCAL,SumatoriaCPFRI;
    nfils = VectorCorrientesTotal.size()-1;
    SumatoriaCP.resize(nfils);
    SumatoriaCPCAL.resize(nfils);
    SumatoriaCPFRI.resize(nfils);
    for(i = 0; i < nfils ; i++){
        SumatoriaCPCAL[i] = 0;
        for(j = 0 ; j < sumCpCalTab[0].size();j++){
            SumatoriaCPCAL[i] = SumatoriaCPCAL[i] + sumCpCalTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCPFRI[i] = 0;
        for(j = 0 ; j < sumCpFriTab[0].size(); j++){
            SumatoriaCPFRI[i] = SumatoriaCPFRI[i] + sumCpFriTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCP[i] = SumatoriaCPFRI[i] - SumatoriaCPCAL[i] ;
    }
    QVector<double> DifTemperaturas;
    nfils = VectorCorrientesTotal.size()-1;
    DifTemperaturas.resize(nfils);
    j = 1;
    for(i = 0; i < VectorCorrientesTotal.size() ; ++i){
        if (j < VectorCorrientesTotal.size()){
            DifTemperaturas[i] = VectorCorrientesTotal[i][1] - VectorCorrientesTotal[j][1];
            ++j;
        }else if(j == VectorCorrientesTotal.size()){
            break;
        }
    }
    QVector<double> Deficit;
    nfils = DifTemperaturas.size();
    Deficit.resize(DifTemperaturas.size());
    for( i = 0 ; i < nfils; ++i){
        Deficit[i] = DifTemperaturas[i] * SumatoriaCP[i];
    }
    int Ldef = Deficit.size();
    QVector<QVector<double>> VecHeatFlow;
    QVector<double> VAL1,VAL2;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecHeatFlow.resize(nfils);
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecHeatFlow[i].resize(ncols);
    }
    VAL1[0]=0;
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecHeatFlow[i][0] = VAL1[i]; // Val1
        VecHeatFlow[i][1] = VAL2[i]; // Val2
    }
    double min = *std::min_element(VAL2.begin(),VAL2.end());
    VAL1.clear();
    VAL2.clear();
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    QVector<QVector<double>> VecAdjHeatFlow;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecAdjHeatFlow.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecAdjHeatFlow[i].resize(ncols);
    }
    VAL1[0]= qFabs(min);
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecAdjHeatFlow[i][0] = VAL1[i]; // Val1
        VecAdjHeatFlow[i][1] = VAL2[i]; // Val2
    }
    // PUNTOS DE LA CURVAS COMPUESTAS
    QVector<double> TGC;
    nfils = VectorCorrientesTotal.size();
    TGC.resize(nfils);
    for (i = 0; i < nfils; i++){
        TGC[i] = (VectorCorrientesTotal[i][0] + VectorCorrientesTotal[i][1])/2;
    }
    QVector<double> EGC;
    EGC.resize(nfils);
    j=0;
    for (i = 0; i < nfils; i++){
        if(i == 0){
            EGC[i] = VecAdjHeatFlow[0][0];
        }else{
            EGC[i] = VecAdjHeatFlow[j][1];
            j++;
        }
    }
    MGCTEMPERATURAS.resize(TGC.size());
    MGCENTALPIA.resize(EGC.size());
    for(i=0;i < TGC.size();i++){
        MGCTEMPERATURAS[i] = TGC[i];
        MGCENTALPIA[i] = EGC[i];
    }
}

QVector<double> PlotGCC_DIVERSA::getGCTEMPERATURAS()
{
    return MGCTEMPERATURAS;
}

QVector<double> PlotGCC_DIVERSA::getGCENTALPIA()
{
    return MGCENTALPIA;
}

double PlotGCC_DIVERSA::getK()
{
    return MK;
}

Plot_Dtmin_vs_Areas_DIVERSO::Plot_Dtmin_vs_Areas_DIVERSO(QVector<double> Tsupply, QVector<double> Ttarget,
                                                         QVector<double> Cp, QVector<double> h,
                                                         QVector<double> Calentamiento,
                                                         QVector<double> Enfriamento,
                                                         double K, double DTmin,float punto1, float punto2)
{
    int i,j,n,nfils,ncols,Pares,contador,k;
    QVector<double> VectorCalientes, VectorCalientesCp;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas;
    QVector<double> DeltasTi;
    Plot_curvascompuestasajustadas plot1(Tsupply,Ttarget,Cp,DTmin);
    QVector<QVector<double>> VADHF = plot1.getVecAdjHeatFlow();
    float UTILIDADCALENTAMIENTO = VADHF[0][0];
    float UTILIDADFRIA = VADHF[VADHF.size()-1][1];
    double ERROR;
    float error1,error2,error3,error = 100.00, errorPasado = 100.00,tolerancia = 0.000001;
    QVector<float> ValoresdeK,ValoresdeError;
    ValoresdeK.resize(31);
    ValoresdeError.resize(31);
    i = 0;
    int iterador = 0;
    int min_pos;
    while(error >= tolerancia ){//while(error >= tolerancia ){
        float incremento = (punto2-punto1)/30.0;
        ValoresdeK[0] = double (punto1);
        for(int i = 1; i <= 30; i++){
            ValoresdeK[i] = float (punto1 + incremento);
            punto1 = punto1 + incremento;
        }
        for(int i = 0; i < ValoresdeK.size(); i++){
            K = double (ValoresdeK[i]);
            FindKvalue Ki(Tsupply,Ttarget,Cp,h,K,DTmin);
            float KiUC = Ki.getUC();
            float KiUF = Ki.getUF();
            error1 = qFabs( ((UTILIDADCALENTAMIENTO - KiUC)/KiUC));
            error2 = qFabs( ((UTILIDADFRIA - KiUF )/KiUF ));
            error3 = error1 + error2;
            ValoresdeError[i] = float (error3);
        }
        min_pos = std::distance(ValoresdeError.begin(),std::min_element(ValoresdeError.begin(),ValoresdeError.end()));
        ERROR = *std::min_element(ValoresdeError.begin(),ValoresdeError.end()); //double
        error = float (ERROR);
        if(error == errorPasado){
            break;
        }
        errorPasado = error;
        //qDebug() <<  "error" << error  << "Valor de K" << ValoresdeK[min_pos] << iterador << min_pos;
        if(error > tolerancia){
            punto1 = ValoresdeK[min_pos]-incremento;
            punto2 = ValoresdeK[min_pos]+incremento;
            iterador = iterador +1;
        }
    }
    MK = ValoresdeK[min_pos];
    K = ValoresdeK[min_pos];
    n = Tsupply.size();
    DeltasTi.resize(n);
    Calentamiento[0] = Calentamiento[0] + (DTmin/2 - (K/(Calentamiento[2]/1000)) ) ;
    Calentamiento[1] = Calentamiento[1] + (DTmin/2 - (K/(Calentamiento[2]/1000)) ) ;
    Enfriamento[0] = Enfriamento[0] - (DTmin/2 - (K/(Enfriamento[2]/1000)) ) ;
    Enfriamento[1] = Enfriamento[1] - (DTmin/2 - (K/(Enfriamento[2]/1000)) );
    for (i = 0; i < n; i++){
        DeltasTi[i] = (K / (h[i]/1000));
    }
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            DeltasTi[i] = (K / (h[i]/1000));
            Tsupply[i] = Tsupply[i] + (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] + (DTmin/2 - DeltasTi[i]);
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] << h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            DeltasTi[i] = (K / (h[i]/1000));
            Tsupply[i] = Tsupply[i] - (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] - (DTmin/2 - DeltasTi[i]);
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] << h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    QVector < QVector <double> > VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/4;
    ncols = 4;
    VectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/4;
    ncols = 4;
    VectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VCC,VFC;
    Pares = VectorCalientes.size();
    nfils = Pares/2;
    ncols = 2;
    VCC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VCC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VCC[i][0] = VectorCalientes[k];
        k++;
        VCC[i][1] = VectorCalientes[k];
        k++;
    }
    Pares = VectorFrias.size();
    nfils = Pares/2;
    ncols = 2;
    VFC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VFC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VFC[i][0] = VectorFrias[k];
        k++;
        VFC[i][1] = VectorFrias[k];
        k++;
    }
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    QVector<double> VCCurvasT = VCCurvas;
    QVector<double> VFCurvasT = VFCurvas;
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size(); // tenia 2
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l=0;
    int m =VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0]-.1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-.1 )
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size(); // AQUI DEBE SE VECTORFIRASMATRIZ.SIZE() tenia 2
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-.1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-.1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    //MODIFICADO
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val1;
    n = CpcurvaCold.size();
    for(i=0; i < n ; i++){
        Val1 << RangosFrias[i][2] * CpcurvaCold[i];
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    // HASTA AQUI ES PARA CURVAS COMPUESTAS
    QVector<QVector<double>> VectorCalientesTmin;
    VectorCalientesTmin.resize(VCCurvasT.size());
    nfils = VCCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i].resize(ncols);
    }
    k = VCCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i][0] = VCCurvasT[k]-DTmin;
        VectorCalientesTmin[i][1] = VCCurvasT[k];
        --k;
    }
    QVector<QVector<double>> VectorFriasTmin;
    VectorFriasTmin.resize(VFCurvasT.size());
    nfils = VFCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i].resize(ncols);
    }
    k = VFCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i][0] = VFCurvasT[k];
        VectorFriasTmin[i][1] = VFCurvasT[k]+DTmin;
        --k;
    }
    QVector<QVector<double>> VectorCorrientesTotal;
    nfils = VCCurvasT.size() + VFCurvasT.size();
    ncols = 2;
    VectorCorrientesTotal.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotal[i].resize(ncols);
    }
    j=0;
    k=0;
    for(i = 0; i < nfils ; i++){
        if(i < VectorCalientesTmin.size()){
            VectorCorrientesTotal[i][0] = VectorCalientesTmin[j][0];
            VectorCorrientesTotal[i][1] = VectorCalientesTmin[j][1];
            ++j;
        }else{
            VectorCorrientesTotal[i][0] = VectorFriasTmin[k][0];
            VectorCorrientesTotal[i][1] = VectorFriasTmin[k][1];
            ++k;
        }
    }
    std::sort(VectorCorrientesTotal.begin(),VectorCorrientesTotal.end());
    QVector<QVector<double>> VectorCorrientesTotalOrdenar;
    nfils = VectorCorrientesTotal.size();
    ncols = 2;
    VectorCorrientesTotalOrdenar.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i].resize(ncols);
    }
    k = VectorCorrientesTotal.size()-1;
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i][0] = VectorCorrientesTotal[k][0];
        VectorCorrientesTotalOrdenar[i][1] = VectorCorrientesTotal[k][1];
        --k;
    }
    VectorCorrientesTotal = VectorCorrientesTotalOrdenar;
    int r3 = VectorCorrientesTotal.size();
    int r4 = VCC.size();
    //FORMAR LA MATRIZ PARA SUMCPCAL
    QVector<QVector<double>> sumCpCalTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VCC.size();//2;
    sumCpCalTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpCalTab[i].resize(ncols);
    }
    l = 0;
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r4; ++j){
            if(VectorCorrientesTotal[i][1] <= VCC[j][0]-.1 && VectorCorrientesTotal[i][1] >= VCC[j][1] -.1 ){
                sumCpCalTab[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCalTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<QVector<double>> sumCpFriTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VFC.size();//2;
    sumCpFriTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpFriTab[i].resize(ncols);
    }
    l = 0;
    int r5 = VFC.size();
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r5; ++j){
            if(VectorCorrientesTotal[i][0] <= VFC[j][1]-.1 && VectorCorrientesTotal[i][0] >= VFC[j][0] -.1){
                sumCpFriTab[l][k] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFriTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<double> SumatoriaCP,SumatoriaCPCAL,SumatoriaCPFRI;
    nfils = VectorCorrientesTotal.size()-1;
    SumatoriaCP.resize(nfils);
    SumatoriaCPCAL.resize(nfils);
    SumatoriaCPFRI.resize(nfils);
    //MODIFICADO
    for(i = 0; i < nfils ; i++){
        SumatoriaCPCAL[i] = 0;
        for(j = 0 ; j < sumCpCalTab[0].size();j++){
            SumatoriaCPCAL[i] = SumatoriaCPCAL[i] + sumCpCalTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCPFRI[i] = 0;
        for(j = 0 ; j < sumCpFriTab[0].size(); j++){
            SumatoriaCPFRI[i] = SumatoriaCPFRI[i] + sumCpFriTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCP[i] = SumatoriaCPFRI[i]- SumatoriaCPCAL[i] ;
    }
    QVector<double> DifTemperaturas;
    nfils = VectorCorrientesTotal.size()-1;
    DifTemperaturas.resize(nfils);
    j = 1;
    for(i = 0; i < VectorCorrientesTotal.size() ; ++i){
        if (j < VectorCorrientesTotal.size()){
            DifTemperaturas[i] = VectorCorrientesTotal[i][1] - VectorCorrientesTotal[j][1];
            ++j;
        }else if(j == VectorCorrientesTotal.size()){
            break;
        }
    }
    //QVector<double> Deficit;
    nfils = DifTemperaturas.size();
    QVector<double> Deficit;
    Deficit.resize(DifTemperaturas.size());
    for( i = 0 ; i < nfils; ++i){
        Deficit[i] = DifTemperaturas[i] * SumatoriaCP[i];
    }
    int Ldef = Deficit.size();
    QVector<QVector<double>> VecHeatFlow;
    QVector<double> VAL1,VAL2;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecHeatFlow.resize(nfils);
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecHeatFlow[i].resize(ncols);
    }
    VAL1[0]=0;
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecHeatFlow[i][0] = VAL1[i]; // Val1
        VecHeatFlow[i][1] = VAL2[i]; // Val2
    }
    double min = *std::min_element(VAL2.begin(),VAL2.end());
    VAL1.clear();
    VAL2.clear();
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    QVector<QVector<double>> VecAdjHeatFlow;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecAdjHeatFlow.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecAdjHeatFlow[i].resize(ncols);
    }
    VAL1[0]= qFabs(min);
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecAdjHeatFlow[i][0] = VAL1[i]; // Val1
        VecAdjHeatFlow[i][1] = VAL2[i]; // Val2
    }
    QVector<double> PuntosCurvasC,PuntosCurvasF;
    n = PUNTOSCC.size();
    PuntosCurvasC.resize(PUNTOSCC.size()+1);
    PuntosCurvasC[0] = 0;
    for(i = 0 ; i < n; i++){
        PuntosCurvasC[i+1] = PuntosCurvasC[i] + PUNTOSCC[i][3];
    }
    n = PUNTOSCF.size();
    PuntosCurvasF.resize(PUNTOSCF.size()+1);
    double max = VecAdjHeatFlow[VecAdjHeatFlow.size()-1][1];
    PuntosCurvasF[0] = max ;
    for(i = 0; i < n; i++){
        PuntosCurvasF[i+1] = PuntosCurvasF[i] + PUNTOSCF[i][3];
    }
    QVector<QVector<double>> CURVA_COMPUESTA_CALIENTE , CCALIENTE;
    QVector<QVector<double>> CURVA_COMPUESTA_FRIA, CCFRIA;
    nfils = PuntosCurvasC.size();
    ncols = 2;
    CURVA_COMPUESTA_CALIENTE.resize(nfils);
    CCALIENTE.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i].resize(ncols);
        CCALIENTE[i].resize(ncols);
    }
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i][0] = VCCurvas[i] - DTmin/2;
        CURVA_COMPUESTA_CALIENTE[i][1] = PuntosCurvasC[i];
        CCALIENTE[i][0] = VCCurvas[i] - DTmin/2;
        CCALIENTE[i][1] = PuntosCurvasC[i];
    }
    nfils = PuntosCurvasF.size();
    ncols = 2;
    CURVA_COMPUESTA_FRIA.resize(nfils);
    CCFRIA.resize(nfils);
    for( i = 0; i < nfils; i++ ){
        CURVA_COMPUESTA_FRIA[i].resize(ncols);
        CCFRIA[i].resize(ncols);
    }
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_FRIA[i][0] = VFCurvas[i] + DTmin/2;
        CURVA_COMPUESTA_FRIA[i][1] = PuntosCurvasF[i];
        CCFRIA[i][0] = VFCurvas[i] + DTmin/2;
        CCFRIA[i][1] = PuntosCurvasF[i];
    }
    QVector<QVector<double>> ACON;
    QVector<double> ACON_ENTALPIA;
    nfils = PuntosCurvasC.size() + PuntosCurvasF.size();
    ncols = 2;
    ACON.resize(nfils);
    ACON_ENTALPIA.resize(nfils);
    for(i = 0 ; i < nfils ; i++){
        ACON[i].resize(ncols);
    }
    j=0;k=0;
    for(i = 0; i < nfils ; i++){
        if(i < PuntosCurvasC.size()){
            ACON[i][0] = VCCurvas[j];
            ACON[i][1] = PuntosCurvasC[j];
            ACON_ENTALPIA[i] = PuntosCurvasC[j];
            j++;
        }else{
            ACON[i][0] = VFCurvas[k];
            ACON[i][1] = PuntosCurvasF[k];
            ACON_ENTALPIA[i] = PuntosCurvasF[k];
            k++;
        }
    }
    std::sort(ACON.begin(),ACON.end());
    std::sort(ACON_ENTALPIA.begin(),ACON_ENTALPIA.end());
    QVector<double> Dif_Intervalos_de_Entalpia;
    QVector<double> Intervalos_de_Entalpia;
    QVector<double> VALOR_INI;
    nfils = ACON_ENTALPIA.size();
    VALOR_INI.resize(nfils);
    Dif_Intervalos_de_Entalpia.resize(nfils-1);
    Intervalos_de_Entalpia.resize(nfils-1);
    VALOR_INI[0] = ACON_ENTALPIA[0];
    j=0;
    for ( i = 0 ; i < nfils-1; ++i){
        Dif_Intervalos_de_Entalpia[i] = qFabs(VALOR_INI[i] - ACON_ENTALPIA[i+1]);
        Intervalos_de_Entalpia[j] = Dif_Intervalos_de_Entalpia[i];
        VALOR_INI[i+1] = std::accumulate(Intervalos_de_Entalpia.begin(),Intervalos_de_Entalpia.end(),0);
        ++j;
    }
    QVector<QVector<double>> InterpolacionCalientes;
    int r1 =  CURVA_COMPUESTA_CALIENTE.size();
    int r2 = CURVA_COMPUESTA_FRIA.size();
    for(i = 0; i < r2 ; i++){
        for(k = 0; k < r1 ; k++){
            if(k==(r1-1)){
                break;
            }
            if(CURVA_COMPUESTA_FRIA[i][1] > CURVA_COMPUESTA_CALIENTE[k][1] &&
                    CURVA_COMPUESTA_FRIA[i][1] < CURVA_COMPUESTA_CALIENTE[k+1][1]){
                    InterpolacionCalientes.resize(1);
                    InterpolacionCalientes[0].resize(2);
                    InterpolacionCalientes[0][0] = CURVA_COMPUESTA_CALIENTE[k][0] +
                            ((CURVA_COMPUESTA_CALIENTE[k+1][0]-CURVA_COMPUESTA_CALIENTE[k][0])
                            /(CURVA_COMPUESTA_CALIENTE[k+1][1]-CURVA_COMPUESTA_CALIENTE[k][1]))*
                            (CURVA_COMPUESTA_FRIA[i][1]-CURVA_COMPUESTA_CALIENTE[k][1]);;
                    InterpolacionCalientes[0][1] = CURVA_COMPUESTA_FRIA[i][1] ;
                    CURVA_COMPUESTA_CALIENTE = CURVA_COMPUESTA_CALIENTE + InterpolacionCalientes;
                    InterpolacionCalientes.clear();
                    k = 0;
                    r1 = CURVA_COMPUESTA_CALIENTE.size();
                    j++;
                    break;
            }
        }
    }
    InterpolacionCalientes.resize(CURVA_COMPUESTA_CALIENTE.size());
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        InterpolacionCalientes[i].resize(2);
    }
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_CALIENTE.at(i).size() ; j++){
            InterpolacionCalientes[i][j] = CURVA_COMPUESTA_CALIENTE[i][j];
        }
    }
    CURVA_COMPUESTA_CALIENTE.clear();
    nfils = CCALIENTE.size();
    ncols = 2;
    CURVA_COMPUESTA_CALIENTE.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i].resize(ncols);
    }
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_CALIENTE.at(i).size() ; j++){
             CURVA_COMPUESTA_CALIENTE[i][j] = CCALIENTE[i][j];
        }
    }
    std::sort(InterpolacionCalientes.begin(),InterpolacionCalientes.end());
    QVector<QVector<double>> InterpolacionFrias;
    r1 =  CURVA_COMPUESTA_FRIA.size();
    r2 = CURVA_COMPUESTA_CALIENTE.size();
    for(i = 0; i < r2 ; i++){
        for(k = 0; k < r1 ; k++){
            if(k==(r1-1)){
                break;
            }
            if(CURVA_COMPUESTA_CALIENTE[i][1]>CURVA_COMPUESTA_FRIA[k][1]
                    && CURVA_COMPUESTA_CALIENTE[i][1]<CURVA_COMPUESTA_FRIA[k+1][1]){
                InterpolacionFrias.resize(1);
                InterpolacionFrias[0].resize(2);
                InterpolacionFrias[0][0] = CURVA_COMPUESTA_FRIA[k][0]+
                        ((CURVA_COMPUESTA_FRIA[k+1][0]-CURVA_COMPUESTA_FRIA[k][0])
                        /(CURVA_COMPUESTA_FRIA[k+1][1]-CURVA_COMPUESTA_FRIA[k][1]))*
                        (CURVA_COMPUESTA_CALIENTE[i][1]-CURVA_COMPUESTA_FRIA[k][1]);
                InterpolacionFrias[0][1] = CURVA_COMPUESTA_CALIENTE[i][1] ;
                CURVA_COMPUESTA_FRIA = CURVA_COMPUESTA_FRIA + InterpolacionFrias;
                InterpolacionFrias.clear();
                k = 0;
                r1 = CURVA_COMPUESTA_FRIA.size();
                j++;
                break;
            }
        }
    }
    InterpolacionFrias.resize(CURVA_COMPUESTA_FRIA.size());
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        InterpolacionFrias[i].resize(2);
    }
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_FRIA.at(i).size() ; j++){
            InterpolacionFrias[i][j] = CURVA_COMPUESTA_FRIA[i][j];
        }
    }
    CURVA_COMPUESTA_FRIA.clear();
    nfils = CCFRIA.size();
    ncols = 2;
    CURVA_COMPUESTA_FRIA.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_FRIA[i].resize(ncols);
    }
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_FRIA.at(i).size() ; j++){
             CURVA_COMPUESTA_FRIA[i][j] = CCFRIA[i][j];
        }
    }
    std::sort(InterpolacionFrias.begin(),InterpolacionFrias.end());
    QVector<double> suma_inter_serfri;
    suma_inter_serfri.resize(CURVA_COMPUESTA_CALIENTE.size());
    for ( i = 0; i <  CURVA_COMPUESTA_CALIENTE.size() ; i++){
        if(CURVA_COMPUESTA_FRIA[0][1] > CURVA_COMPUESTA_CALIENTE[i][1]){
            suma_inter_serfri[i] = 1;
        }else{
            suma_inter_serfri[i] = 0;
        }
    }
    QVector<double> suma_inter_sercal;
    suma_inter_sercal.resize(CURVA_COMPUESTA_FRIA.size());
    for ( i = 0; i <  CURVA_COMPUESTA_FRIA.size() ; i++){
        if(CURVA_COMPUESTA_FRIA[i][1] > CURVA_COMPUESTA_CALIENTE[CURVA_COMPUESTA_CALIENTE.size()-1][1]){
            suma_inter_sercal[i] = 1;
        }else{
            suma_inter_sercal[i] = 0;
        }
    }
    double SUMA_inter_serfri = std::accumulate(suma_inter_serfri.begin(),suma_inter_serfri.end(),0);
    double SUMA_inter_sercal = std::accumulate(suma_inter_sercal.begin(),suma_inter_sercal.end(),0);
    QVector<QVector<double>> INTERVALOSCALIENTES,INTERVALOSFRIAS;
    nfils = SUMA_inter_sercal + InterpolacionCalientes.size()-1;
    ncols = 2;
    INTERVALOSCALIENTES.resize(nfils);
    for ( i = 0 ; i < nfils ; i++){
        INTERVALOSCALIENTES[i].resize(ncols);
    }
    j=0;
    for ( i = 0 ; i <= nfils ; i++){
        if(i == nfils){
            break;
        }
        if( i < InterpolacionCalientes.size()-1){
            INTERVALOSCALIENTES[i][0] = InterpolacionCalientes[j][0];
            INTERVALOSCALIENTES[i][1] = InterpolacionCalientes[j+1][0];
            j++;
        }else{
            INTERVALOSCALIENTES[i][0] = Calentamiento[0];
            INTERVALOSCALIENTES[i][1] = Calentamiento[1];
        }
    }
    nfils = SUMA_inter_serfri + InterpolacionFrias.size() -1;
    ncols = 2;
    INTERVALOSFRIAS.resize(nfils);
    for ( i = 0 ; i < nfils ; i++){
        INTERVALOSFRIAS[i].resize(ncols);
    }
    j=0;
    for ( i = 0 ; i < nfils ; i++){
        if(i == nfils){
            break;
        }
        if( i < SUMA_inter_serfri){
            INTERVALOSFRIAS[i][0] = Enfriamento[0];
            INTERVALOSFRIAS[i][1] = Enfriamento[1];
        }else{
            INTERVALOSFRIAS[i][0] = InterpolacionFrias[j][0];
            INTERVALOSFRIAS[i][1] = InterpolacionFrias[j+1][0];
            j++;
        }
    }
    QVector<QVector<double>> INTERVALOS_AGRUPADOS;
    nfils = INTERVALOSFRIAS.size();
    ncols = 4;
    INTERVALOS_AGRUPADOS.resize(nfils);
    MINTERVALOS_AGRUPADOS.resize(nfils);
    for ( i = 0; i < nfils ; i++){
        INTERVALOS_AGRUPADOS[i].resize(ncols);
        MINTERVALOS_AGRUPADOS[i].resize(ncols);
    }
    for ( i = 0; i < nfils ; i++){
        INTERVALOS_AGRUPADOS[i][0] = INTERVALOSCALIENTES[i][0];
        INTERVALOS_AGRUPADOS[i][1] = INTERVALOSCALIENTES[i][1];
        INTERVALOS_AGRUPADOS[i][2] = INTERVALOSFRIAS[i][0];
        INTERVALOS_AGRUPADOS[i][3] = INTERVALOSFRIAS[i][1];
        MINTERVALOS_AGRUPADOS[i][0] = INTERVALOSCALIENTES[i][0];
        MINTERVALOS_AGRUPADOS[i][1] = INTERVALOSCALIENTES[i][1];
        MINTERVALOS_AGRUPADOS[i][2] = INTERVALOSFRIAS[i][0];
        MINTERVALOS_AGRUPADOS[i][3] = INTERVALOSFRIAS[i][1];
    }
    QVector<QVector<double>> Calcal,CalcalAreas;
    double Comparacion;
    r1 = INTERVALOSCALIENTES.size(); //CHECAR INTERVALOS_AGRUPADOS.size();
    r2 = VectorCalientesMATRIZ.size();
    Calcal.resize(r1);
    CalcalAreas.resize(r1);
    for( i = 0; i < r1 ; i++){
        Calcal[i].resize(r2);
        CalcalAreas[i].resize(r2);
    }
    for( i = 0; i < r1 ; i++){
        for ( j = 0; j < r2; j++){
            Comparacion = (INTERVALOSCALIENTES[i][0] + INTERVALOSCALIENTES[i][1]) / 2;
            if(Comparacion < VectorCalientesMATRIZ[j][0]  && Comparacion > VectorCalientesMATRIZ[j][1]-.1){
                Calcal[i][j] = (VectorCalientesMATRIZ[j][2]*(INTERVALOSCALIENTES[i][1]-INTERVALOSCALIENTES[i][0]))*((K/VectorCalientesMATRIZ[j][3])*1000);
                CalcalAreas[i][j] = (VectorCalientesMATRIZ[j][2]*(INTERVALOSCALIENTES[i][1]-INTERVALOSCALIENTES[i][0]))/VectorCalientesMATRIZ[j][3];
            }else{
                Calcal[i][j] = 0;
                CalcalAreas[i][j] = 0;
            }
        }
    }
    QVector<QVector<double>> Calfri,CalfriAreas;
    r1 = INTERVALOSFRIAS.size();  //CHECAR INTERVALOSFRIAS.size();
    r2 = VectorCalientesMATRIZ.size();
    Calfri.resize(r1);
    CalfriAreas.resize(r1);
    for( i = 0 ; i < r1; i++){
        Calfri[i].resize(r2);
        CalfriAreas[i].resize(r2);
    }
    for( i = 0; i < r1 ; i++){
        for ( j = 0; j < r2; j++){
            Comparacion = (INTERVALOSFRIAS[i][0] + INTERVALOSFRIAS[i][0]) / 2;
            if(Comparacion < VectorFriasMATRIZ[j][1] && Comparacion > VectorFriasMATRIZ[j][0]-.1){
                Calfri[i][j] = (VectorFriasMATRIZ[j][2]*(INTERVALOSFRIAS[i][1]-INTERVALOSFRIAS[i][0]))*((K/VectorFriasMATRIZ[j][3])*1000);
                CalfriAreas[i][j] = (VectorFriasMATRIZ[j][2]*(INTERVALOSFRIAS[i][1]-INTERVALOSFRIAS[i][0]))/VectorFriasMATRIZ[j][3];
            }else{
                Calfri[i][j] = 0;
                CalfriAreas[i][j] = 0;
            }
        }
    }
    QVector<double> SumatorioQDT, SumatorioQDTAREAS,SUMACAL,SUMACALAREAS,SUMAFRI,SUMAFRIAREAS;
    QVector<double> DeltaTM,DeltaTLM;
    SUMACAL.resize(Calcal.size());
    SUMACALAREAS.resize(Calcal.size());
    SUMAFRI.resize(Calfri.size());
    SUMAFRIAREAS.resize(Calfri.size());
    for(i=0; i < Calcal.size(); i++){
        SUMACAL[i] = 0;
        SUMACALAREAS[i] = 0;
        for(j = 0; j < Calcal[0].size(); j++){
            SUMACAL[i] = SUMACAL[i] + Calcal[i][j];
            SUMACALAREAS[i] = SUMACALAREAS[i] + CalcalAreas[i][j];
        }
    }
    for(i=0; i < Calfri.size(); i++){
        SUMAFRI[i] = 0;
        SUMAFRIAREAS[i] =0;
        for(j = 0; j < Calfri[0].size(); j++){
            SUMAFRI[i] = SUMAFRI[i] + Calfri[i][j];
            SUMAFRIAREAS[i] = SUMAFRIAREAS[i] + CalfriAreas[i][j];
        }
    }
    nfils = Calcal.size();
    SumatorioQDT.resize(Intervalos_de_Entalpia.size());
    SumatorioQDTAREAS.resize(Intervalos_de_Entalpia.size());
    DeltaTM.resize(Intervalos_de_Entalpia.size());
    DeltaTLM.resize(Intervalos_de_Entalpia.size());
    QVector<QVector<double>> DeltaTSF;
    r1 = INTERVALOS_AGRUPADOS.size();
    DeltaTSF.resize(r1);
    for( i = 0 ; i < r1; i++){
        DeltaTSF[i].resize(2);
    }
    for(i = 0; i < r1; i++){
        DeltaTSF[i][0] = qFabs(INTERVALOS_AGRUPADOS[i][0] - INTERVALOS_AGRUPADOS[i][2]);
        DeltaTSF[i][1] = qFabs(INTERVALOS_AGRUPADOS[i][1] - INTERVALOS_AGRUPADOS[i][3]);
    }
    for( i = 0; i < nfils ; i++){
        SumatorioQDT[i] = SUMACAL[i] + SUMAFRI[i];
        SumatorioQDTAREAS[i] = SUMACALAREAS[i] + SUMAFRIAREAS[i];
    }
    int r1CAL = SUMA_inter_sercal;
    int r2FRI = SUMA_inter_serfri;
    for ( i = 0 ; i < r2FRI ; i++){
        SumatorioQDT[i] = SumatorioQDT[i] + Intervalos_de_Entalpia[i]*((K/Enfriamento[2])*1000);
        SumatorioQDTAREAS[i] = SumatorioQDTAREAS[i] + Intervalos_de_Entalpia[i]/Enfriamento[2];
    }
    for ( i = SumatorioQDT.size()-r1CAL; i < SumatorioQDT.size(); i++){
        SumatorioQDT[i] = SumatorioQDT[i] + Intervalos_de_Entalpia[i]*((K/Calentamiento[2])*1000);
        SumatorioQDTAREAS[i] = SumatorioQDTAREAS[i] + Intervalos_de_Entalpia[i]/Calentamiento[2];
    }
    for( i = 0; i < nfils ; i++){
        DeltaTM[i] = SumatorioQDT[i]/ Intervalos_de_Entalpia[i];
        DeltaTSF[i][0] = DeltaTSF[i][0] + DeltaTM[i];
        DeltaTSF[i][1] = DeltaTSF[i][1] + DeltaTM[i];
        DeltaTLM[i] =  (DeltaTSF[i][0] + DeltaTSF[i][1])/2;
    }
    //CALCULO DE AREAS;
    QVector<double> Areas;
    Areas.resize(SumatorioQDTAREAS.size());
    MVectorAreas.resize((SumatorioQDTAREAS.size()));
    MVectorEntalpia.resize((SumatorioQDTAREAS.size()));
    MDeltaTLM.resize((SumatorioQDTAREAS.size()));
    for ( i = 0 ; i < SumatorioQDTAREAS.size(); i++){
        Areas[i] = SumatorioQDTAREAS[i] / DeltaTLM[i];
        MVectorAreas[i] = Areas[i];
        MVectorEntalpia[i] = SumatorioQDTAREAS[i];
        MDeltaTLM[i] = DeltaTLM[i];
    }
    MAREAS = std::accumulate(Areas.begin(),Areas.end(),0);
}

double Plot_Dtmin_vs_Areas_DIVERSO::getAREAS()
{
    return MAREAS;
}

double Plot_Dtmin_vs_Areas_DIVERSO::getK()
{
    return MK;
}

QVector<double> Plot_Dtmin_vs_Areas_DIVERSO::getVectorAreas()
{
    return MVectorAreas;
}

QVector<double> Plot_Dtmin_vs_Areas_DIVERSO::getVectorEntalpia()
{
    return MVectorEntalpia;
}

QVector<double> Plot_Dtmin_vs_Areas_DIVERSO::getDeltaTLM()
{
    return MDeltaTLM;
}

QVector<QVector<double> > Plot_Dtmin_vs_Areas_DIVERSO::getINTERVALOS_AGRUPADOS()
{
    return MINTERVALOS_AGRUPADOS;
}

FindKvalue::FindKvalue(QVector<double> Tsupply, QVector<double> Ttarget,
                       QVector<double> Cp, QVector<double> h,
                       double K ,double DTmin)
{
    QVector<double> VectorCalientes, VectorCalientesCp;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas;
    QVector<double> DeltasTi;
    int i,j,n,nfils,ncols,Pares,contador,k;
    n = Tsupply.size();
    DeltasTi.resize(n);
    for (i = 0; i < n; i++){
        DeltasTi[i] = (K / (h[i]/1000));
    }
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            DeltasTi[i] = (K / (h[i]/1000));
            Tsupply[i] = Tsupply[i] + (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] + (DTmin/2 - DeltasTi[i]);
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] << h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            DeltasTi[i] = (K / (h[i]/1000));
            Tsupply[i] = Tsupply[i] - (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] - (DTmin/2 - DeltasTi[i]);
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] << h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    QVector < QVector <double> > VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/4;
    ncols = 4;
    VectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/4;
    ncols = 4;
    VectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VCC,VFC;
    Pares = VectorCalientes.size();
    nfils = Pares/2;
    ncols = 2;
    VCC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VCC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VCC[i][0] = VectorCalientes[k];
        k++;
        VCC[i][1] = VectorCalientes[k];
        k++;
    }
    Pares = VectorFrias.size();
    nfils = Pares/2;
    ncols = 2;
    VFC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VFC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VFC[i][0] = VectorFrias[k];
        k++;
        VFC[i][1] = VectorFrias[k];
        k++;
    }
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    QVector<double> VCCurvasT = VCCurvas;
    QVector<double> VFCurvasT = VFCurvas;
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size(); // tenia 2
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l=0;
    int m =VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0]-.1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-.1 )
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size(); // AQUI DEBE SE VECTORFIRASMATRIZ.SIZE() tenia 2
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-.1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-.1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    //MODIFICADO
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val1;
    n = CpcurvaCold.size();
    for(i=0; i < n ; i++){
        Val1 << RangosFrias[i][2] * CpcurvaCold[i];
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    // HASTA AQUI ES PARA CURVAS COMPUESTAS
    QVector<QVector<double>> VectorCalientesTmin;
    VectorCalientesTmin.resize(VCCurvasT.size());
    nfils = VCCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i].resize(ncols);
    }
    k = VCCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i][0] = VCCurvasT[k]-DTmin;
        VectorCalientesTmin[i][1] = VCCurvasT[k];
        --k;
    }
    QVector<QVector<double>> VectorFriasTmin;
    VectorFriasTmin.resize(VFCurvasT.size());
    nfils = VFCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i].resize(ncols);
    }
    k = VFCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i][0] = VFCurvasT[k];
        VectorFriasTmin[i][1] = VFCurvasT[k]+DTmin;
        --k;
    }
    QVector<QVector<double>> VectorCorrientesTotal;
    nfils = VCCurvasT.size() + VFCurvasT.size();
    ncols = 2;
    VectorCorrientesTotal.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotal[i].resize(ncols);
    }
    j=0;
    k=0;
    for(i = 0; i < nfils ; i++){
        if(i < VectorCalientesTmin.size()){
            VectorCorrientesTotal[i][0] = VectorCalientesTmin[j][0];
            VectorCorrientesTotal[i][1] = VectorCalientesTmin[j][1];
            ++j;
        }else{
            VectorCorrientesTotal[i][0] = VectorFriasTmin[k][0];
            VectorCorrientesTotal[i][1] = VectorFriasTmin[k][1];
            ++k;
        }
    }
    std::sort(VectorCorrientesTotal.begin(),VectorCorrientesTotal.end());
    QVector<QVector<double>> VectorCorrientesTotalOrdenar;
    nfils = VectorCorrientesTotal.size();
    ncols = 2;
    VectorCorrientesTotalOrdenar.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i].resize(ncols);
    }
    k = VectorCorrientesTotal.size()-1;
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i][0] = VectorCorrientesTotal[k][0];
        VectorCorrientesTotalOrdenar[i][1] = VectorCorrientesTotal[k][1];
        --k;
    }
    VectorCorrientesTotal = VectorCorrientesTotalOrdenar;
    int r3 = VectorCorrientesTotal.size();
    int r4 = VCC.size();
    //FORMAR LA MATRIZ PARA SUMCPCAL
    QVector<QVector<double>> sumCpCalTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VCC.size();//2;
    sumCpCalTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpCalTab[i].resize(ncols);
    }
    l = 0;
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r4; ++j){
            if(VectorCorrientesTotal[i][1] <= VCC[j][0]-.1 && VectorCorrientesTotal[i][1] >= VCC[j][1] -.1 ){
                sumCpCalTab[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCalTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<QVector<double>> sumCpFriTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VFC.size();//2;
    sumCpFriTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpFriTab[i].resize(ncols);
    }
    l = 0;
    int r5 = VFC.size();
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r5; ++j){
            if(VectorCorrientesTotal[i][0] <= VFC[j][1]-.1 && VectorCorrientesTotal[i][0] >= VFC[j][0] -.1){
                sumCpFriTab[l][k] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFriTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<double> SumatoriaCP,SumatoriaCPCAL,SumatoriaCPFRI;
    nfils = VectorCorrientesTotal.size()-1;
    SumatoriaCP.resize(nfils);
    SumatoriaCPCAL.resize(nfils);
    SumatoriaCPFRI.resize(nfils);
    //MODIFICADO
    for(i = 0; i < nfils ; i++){
        SumatoriaCPCAL[i] = 0;
        for(j = 0 ; j < sumCpCalTab[0].size();j++){
            SumatoriaCPCAL[i] = SumatoriaCPCAL[i] + sumCpCalTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCPFRI[i] = 0;
        for(j = 0 ; j < sumCpFriTab[0].size(); j++){
            SumatoriaCPFRI[i] = SumatoriaCPFRI[i] + sumCpFriTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCP[i] = SumatoriaCPFRI[i]- SumatoriaCPCAL[i] ;
    }
    QVector<double> DifTemperaturas;
    nfils = VectorCorrientesTotal.size()-1;
    DifTemperaturas.resize(nfils);
    j = 1;
    for(i = 0; i < VectorCorrientesTotal.size() ; ++i){
        if (j < VectorCorrientesTotal.size()){
            DifTemperaturas[i] = VectorCorrientesTotal[i][1] - VectorCorrientesTotal[j][1];
            ++j;
        }else if(j == VectorCorrientesTotal.size()){
            break;
        }
    }
    //QVector<double> Deficit;
    nfils = DifTemperaturas.size();
    QVector<double> Deficit;
    Deficit.resize(DifTemperaturas.size());
    for( i = 0 ; i < nfils; ++i){
        Deficit[i] = DifTemperaturas[i] * SumatoriaCP[i];
    }
    int Ldef = Deficit.size();
    QVector<QVector<double>> VecHeatFlow;
    QVector<double> VAL1,VAL2;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecHeatFlow.resize(nfils);
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecHeatFlow[i].resize(ncols);
    }
    VAL1[0]=0;
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecHeatFlow[i][0] = VAL1[i]; // Val1
        VecHeatFlow[i][1] = VAL2[i]; // Val2
    }
    double min = *std::min_element(VAL2.begin(),VAL2.end());
    VAL1.clear();
    VAL2.clear();
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    QVector<QVector<double>> VecAdjHeatFlow;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecAdjHeatFlow.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecAdjHeatFlow[i].resize(ncols);
    }
    VAL1[0]= qFabs(min);
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecAdjHeatFlow[i][0] = VAL1[i]; // Val1
        VecAdjHeatFlow[i][1] = VAL2[i]; // Val2
    }
    MUTILIDADCALENTAMIENTO = float (VecAdjHeatFlow[0][0]);
    MUTILIDADFRIA = float (VecAdjHeatFlow[VecAdjHeatFlow.size()-1][1]);
}

float FindKvalue::getUC()
{
    return MUTILIDADCALENTAMIENTO;
}

float FindKvalue::getUF()
{
    return MUTILIDADFRIA;
}


Plot_Costos_vs_Areas_Uniforme::Plot_Costos_vs_Areas_Uniforme(QVector<double> Tsupply, QVector<double> Ttarget,
                                                             QVector<double> Cp, QVector<double> h, QVector<double> Calentamiento,
                                                             QVector<double> Enfriamento, QVector<QVector<double> > CapitalCost,
                                                             QVector<double> OperationCost, double DTmin, int CTo, int CCo,bool SI,bool SIS)
{
    QVector<double> VectorCalientes, VectorCalientesCp;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas;
    int i,j,n,nfils,ncols,Pares,contador,k;
    n = Tsupply.size();
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] << h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] << h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    QVector < QVector <double> > VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/4;
    ncols = 4;
    VectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/4;
    ncols = 4;
    VectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VCC,VFC;
    Pares = VectorCalientes.size();
    nfils = Pares/2;
    ncols = 2;
    VCC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VCC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VCC[i][0] = VectorCalientes[k];
        k++;
        VCC[i][1] = VectorCalientes[k];
        k++;
    }
    Pares = VectorFrias.size();
    nfils = Pares/2;
    ncols = 2;
    VFC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VFC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VFC[i][0] = VectorFrias[k];
        k++;
        VFC[i][1] = VectorFrias[k];
        k++;
    }
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    QVector<double> VCCurvasT = VCCurvas;
    QVector<double> VFCurvasT = VFCurvas;
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size();//2;
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l=0;
    int m =VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0]-.1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-.1 )
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size();//2;
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-.1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-.1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    //MODIFICADO
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val1;
    n = CpcurvaCold.size();
    for(i=0; i < n ; i++){
        Val1 << RangosFrias[i][2] * CpcurvaCold[i];
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    // HASTA AQUI ES PARA CURVAS COMPUESTAS
    QVector<QVector<double>> VectorCalientesTmin;
    VectorCalientesTmin.resize(VCCurvasT.size());
    nfils = VCCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i].resize(ncols);
    }
    k = VCCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i][0] = VCCurvasT[k]-DTmin;
        VectorCalientesTmin[i][1] = VCCurvasT[k];
        --k;
    }
    QVector<QVector<double>> VectorFriasTmin;
    VectorFriasTmin.resize(VFCurvasT.size());
    nfils = VFCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i].resize(ncols);
    }
    k = VFCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i][0] = VFCurvasT[k];
        VectorFriasTmin[i][1] = VFCurvasT[k]+DTmin;
        --k;
    }
    QVector<QVector<double>> VectorCorrientesTotal;
    nfils = VCCurvasT.size() + VFCurvasT.size();
    ncols = 2;
    VectorCorrientesTotal.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotal[i].resize(ncols);
    }
    j=0;
    k=0;
    for(i = 0; i < nfils ; i++){
        if(i < VectorCalientesTmin.size()){
            VectorCorrientesTotal[i][0] = VectorCalientesTmin[j][0];
            VectorCorrientesTotal[i][1] = VectorCalientesTmin[j][1];
            ++j;
        }else{
            VectorCorrientesTotal[i][0] = VectorFriasTmin[k][0];
            VectorCorrientesTotal[i][1] = VectorFriasTmin[k][1];
            ++k;
        }
    }
    std::sort(VectorCorrientesTotal.begin(),VectorCorrientesTotal.end());
    QVector<QVector<double>> VectorCorrientesTotalOrdenar;
    nfils = VectorCorrientesTotal.size();
    ncols = 2;
    VectorCorrientesTotalOrdenar.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i].resize(ncols);
    }
    k = VectorCorrientesTotal.size()-1;
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i][0] = VectorCorrientesTotal[k][0];
        VectorCorrientesTotalOrdenar[i][1] = VectorCorrientesTotal[k][1];
        --k;
    }
    VectorCorrientesTotal = VectorCorrientesTotalOrdenar;
    int r3 = VectorCorrientesTotal.size();
    int r4 = VCC.size();
    //FORMAR LA MATRIZ PARA SUMCPCAL
    QVector<QVector<double>> sumCpCalTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VCC.size();//2;
    sumCpCalTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpCalTab[i].resize(ncols);
    }
    l = 0;
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r4; ++j){
            if(VectorCorrientesTotal[i][1] <= VCC[j][0]-.1 && VectorCorrientesTotal[i][1] >= VCC[j][1] -.1 ){
                sumCpCalTab[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCalTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<QVector<double>> sumCpFriTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VFC.size();//2;
    sumCpFriTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpFriTab[i].resize(ncols);
    }
    l = 0;
    int r5 = VFC.size();
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r5; ++j){
            if(VectorCorrientesTotal[i][0] <= VFC[j][1]-.1 && VectorCorrientesTotal[i][0] >= VFC[j][0] -.1){
                sumCpFriTab[l][k] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFriTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<double> SumatoriaCP,SumatoriaCPCAL,SumatoriaCPFRI;
    nfils = VectorCorrientesTotal.size()-1;
    SumatoriaCP.resize(nfils);
    SumatoriaCPCAL.resize(nfils);
    SumatoriaCPFRI.resize(nfils);
    //MODIFICADO
    for(i = 0; i < nfils ; i++){
        SumatoriaCPCAL[i] = 0;
        for(j = 0 ; j < sumCpCalTab[0].size();j++){
            SumatoriaCPCAL[i] = SumatoriaCPCAL[i] + sumCpCalTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCPFRI[i] = 0;
        for(j = 0 ; j < sumCpFriTab[0].size(); j++){
            SumatoriaCPFRI[i] = SumatoriaCPFRI[i] + sumCpFriTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCP[i] = SumatoriaCPFRI[i] - SumatoriaCPCAL[i] ;
    }
    QVector<double> DifTemperaturas;
    nfils = VectorCorrientesTotal.size()-1;
    DifTemperaturas.resize(nfils);
    j = 1;
    for(i = 0; i < VectorCorrientesTotal.size() ; ++i){
        if (j < VectorCorrientesTotal.size()){
            DifTemperaturas[i] = VectorCorrientesTotal[i][1] - VectorCorrientesTotal[j][1];
            ++j;
        }else if(j == VectorCorrientesTotal.size()){
            break;
        }
    }
    QVector<double> Deficit;
    nfils = DifTemperaturas.size();
    Deficit.resize(DifTemperaturas.size());
    for( i = 0 ; i < nfils; ++i){
        Deficit[i] = DifTemperaturas[i] * SumatoriaCP[i];
    }
    int Ldef = Deficit.size();
    QVector<QVector<double>> VecHeatFlow;
    QVector<double> VAL1,VAL2;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecHeatFlow.resize(nfils);
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecHeatFlow[i].resize(ncols);
    }
    VAL1[0]=0;
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecHeatFlow[i][0] = VAL1[i]; // Val1
        VecHeatFlow[i][1] = VAL2[i]; // Val2
    }
    double min = *std::min_element(VAL2.begin(),VAL2.end());
    VAL1.clear();
    VAL2.clear();
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    QVector<QVector<double>> VecAdjHeatFlow;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecAdjHeatFlow.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecAdjHeatFlow[i].resize(ncols);
    }
    VAL1[0]= qFabs(min);
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecAdjHeatFlow[i][0] = VAL1[i]; // Val1
        VecAdjHeatFlow[i][1] = VAL2[i]; // Val2
    }
    QVector<double> PuntosCurvasC,PuntosCurvasF;
    n = PUNTOSCC.size();
    PuntosCurvasC.resize(PUNTOSCC.size()+1);
    PuntosCurvasC[0] = 0;
    for(i = 0 ; i < n; i++){
        PuntosCurvasC[i+1] = PuntosCurvasC[i] + PUNTOSCC[i][3];
    }
    n = PUNTOSCF.size();
    PuntosCurvasF.resize(PUNTOSCF.size()+1);
    //double max = *std::max_element(VAL2.begin(),VAL2.end());
    double max = VecAdjHeatFlow[VecAdjHeatFlow.size()-1][1];
    PuntosCurvasF[0] = max ;
    for(i = 0; i < n; i++){
        PuntosCurvasF[i+1] = PuntosCurvasF[i] + PUNTOSCF[i][3];
    }
    QVector<QVector<double>> CURVA_COMPUESTA_CALIENTE , CCALIENTE;
    QVector<QVector<double>> CURVA_COMPUESTA_FRIA, CCFRIA;
    nfils = PuntosCurvasC.size();
    ncols = 2;
    CURVA_COMPUESTA_CALIENTE.resize(nfils);
    CCALIENTE.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i].resize(ncols);
        CCALIENTE[i].resize(ncols);
    }
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i][0] = VCCurvas[i];
        CURVA_COMPUESTA_CALIENTE[i][1] = PuntosCurvasC[i];
        CCALIENTE[i][0] = VCCurvas[i];
        CCALIENTE[i][1] = PuntosCurvasC[i];
    }
    nfils = PuntosCurvasF.size();
    ncols = 2;
    CURVA_COMPUESTA_FRIA.resize(nfils);
    CCFRIA.resize(nfils);
    for( i = 0; i < nfils; i++ ){
        CURVA_COMPUESTA_FRIA[i].resize(ncols);
        CCFRIA[i].resize(ncols);
    }
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_FRIA[i][0] = VFCurvas[i];
        CURVA_COMPUESTA_FRIA[i][1] = PuntosCurvasF[i];
        CCFRIA[i][0] = VFCurvas[i];
        CCFRIA[i][1] = PuntosCurvasF[i];
    }
    QVector<QVector<double>> ACON;
    QVector<double> ACON_ENTALPIA;
    nfils = PuntosCurvasC.size() + PuntosCurvasF.size();
    ncols = 2;
    ACON.resize(nfils);
    ACON_ENTALPIA.resize(nfils);
    for(i = 0 ; i < nfils ; i++){
        ACON[i].resize(ncols);
    }
    j=0;k=0;
    for(i = 0; i < nfils ; i++){
        if(i < PuntosCurvasC.size()){
            ACON[i][0] = VCCurvas[j];
            ACON[i][1] = PuntosCurvasC[j];
            ACON_ENTALPIA[i] = PuntosCurvasC[j];
            j++;
        }else{
            ACON[i][0] = VFCurvas[k];
            ACON[i][1] = PuntosCurvasF[k];
            ACON_ENTALPIA[i] = PuntosCurvasF[k];
            k++;
        }
    }
    std::sort(ACON.begin(),ACON.end());
    std::sort(ACON_ENTALPIA.begin(),ACON_ENTALPIA.end());
    QVector<double> Dif_Intervalos_de_Entalpia;
    QVector<double> Intervalos_de_Entalpia;
    QVector<double> VALOR_INI;
    nfils = ACON_ENTALPIA.size();
    VALOR_INI.resize(nfils);
    Dif_Intervalos_de_Entalpia.resize(nfils-1);
    Intervalos_de_Entalpia.resize(nfils-1);
    VALOR_INI[0] = ACON_ENTALPIA[0];
    j=0;
    for ( i = 0 ; i < nfils-1; ++i){
        Dif_Intervalos_de_Entalpia[i] = qFabs(VALOR_INI[i] - ACON_ENTALPIA[i+1]);
        Intervalos_de_Entalpia[j] = Dif_Intervalos_de_Entalpia[i];
        VALOR_INI[i+1] = std::accumulate(Intervalos_de_Entalpia.begin(),Intervalos_de_Entalpia.end(),0);
        ++j;
    }
    QVector<QVector<double>> InterpolacionCalientes;
    int r1 =  CURVA_COMPUESTA_CALIENTE.size();
    int r2 = CURVA_COMPUESTA_FRIA.size();
    for(i = 0; i < r2 ; i++){
        for(k = 0; k < r1 ; k++){
            if(k==(r1-1)){
                break;
            }
            if(CURVA_COMPUESTA_FRIA[i][1] > CURVA_COMPUESTA_CALIENTE[k][1] &&
                    CURVA_COMPUESTA_FRIA[i][1] < CURVA_COMPUESTA_CALIENTE[k+1][1]){
                    InterpolacionCalientes.resize(1);
                    InterpolacionCalientes[0].resize(2);
                    InterpolacionCalientes[0][0] = CURVA_COMPUESTA_CALIENTE[k][0] +
                            ((CURVA_COMPUESTA_CALIENTE[k+1][0]-CURVA_COMPUESTA_CALIENTE[k][0])
                            /(CURVA_COMPUESTA_CALIENTE[k+1][1]-CURVA_COMPUESTA_CALIENTE[k][1]))*
                            (CURVA_COMPUESTA_FRIA[i][1]-CURVA_COMPUESTA_CALIENTE[k][1]);;
                    InterpolacionCalientes[0][1] = CURVA_COMPUESTA_FRIA[i][1] ;
                    CURVA_COMPUESTA_CALIENTE = CURVA_COMPUESTA_CALIENTE + InterpolacionCalientes;
                    InterpolacionCalientes.clear();
                    k = 0;
                    r1 = CURVA_COMPUESTA_CALIENTE.size();
                    j++;
                    break;
            }
        }
    }
    InterpolacionCalientes.resize(CURVA_COMPUESTA_CALIENTE.size());
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        InterpolacionCalientes[i].resize(2);
    }
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_CALIENTE.at(i).size() ; j++){
            InterpolacionCalientes[i][j] = CURVA_COMPUESTA_CALIENTE[i][j];
        }
    }
    CURVA_COMPUESTA_CALIENTE.clear();
    nfils = CCALIENTE.size();
    ncols = 2;
    CURVA_COMPUESTA_CALIENTE.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i].resize(ncols);
    }
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_CALIENTE.at(i).size() ; j++){
             CURVA_COMPUESTA_CALIENTE[i][j] = CCALIENTE[i][j];
        }
    }
    std::sort(InterpolacionCalientes.begin(),InterpolacionCalientes.end());
    QVector<QVector<double>> InterpolacionFrias;
    r1 =  CURVA_COMPUESTA_FRIA.size();
    r2 = CURVA_COMPUESTA_CALIENTE.size();
    for(i = 0; i < r2 ; i++){
        for(k = 0; k < r1 ; k++){
            if(k==(r1-1)){
                break;
            }
            if(CURVA_COMPUESTA_CALIENTE[i][1]>CURVA_COMPUESTA_FRIA[k][1]
                    && CURVA_COMPUESTA_CALIENTE[i][1]<CURVA_COMPUESTA_FRIA[k+1][1]){
                InterpolacionFrias.resize(1);
                InterpolacionFrias[0].resize(2);
                InterpolacionFrias[0][0] = CURVA_COMPUESTA_FRIA[k][0]+
                        ((CURVA_COMPUESTA_FRIA[k+1][0]-CURVA_COMPUESTA_FRIA[k][0])
                        /(CURVA_COMPUESTA_FRIA[k+1][1]-CURVA_COMPUESTA_FRIA[k][1]))*
                        (CURVA_COMPUESTA_CALIENTE[i][1]-CURVA_COMPUESTA_FRIA[k][1]);
                InterpolacionFrias[0][1] = CURVA_COMPUESTA_CALIENTE[i][1] ;
                CURVA_COMPUESTA_FRIA = CURVA_COMPUESTA_FRIA + InterpolacionFrias;
                InterpolacionFrias.clear();
                k = 0;
                r1 = CURVA_COMPUESTA_FRIA.size();
                j++;
                break;
            }
        }
    }
    InterpolacionFrias.resize(CURVA_COMPUESTA_FRIA.size());
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        InterpolacionFrias[i].resize(2);
    }
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_FRIA.at(i).size() ; j++){
            InterpolacionFrias[i][j] = CURVA_COMPUESTA_FRIA[i][j];
        }
    }
    CURVA_COMPUESTA_FRIA.clear();
    nfils = CCFRIA.size();
    ncols = 2;
    CURVA_COMPUESTA_FRIA.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_FRIA[i].resize(ncols);
    }
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_FRIA.at(i).size() ; j++){
             CURVA_COMPUESTA_FRIA[i][j] = CCFRIA[i][j];
        }
    }
    std::sort(InterpolacionFrias.begin(),InterpolacionFrias.end());
    QVector<double> suma_inter_serfri;
    suma_inter_serfri.resize(CURVA_COMPUESTA_CALIENTE.size());
    for ( i = 0; i <  CURVA_COMPUESTA_CALIENTE.size() ; i++){
        if(CURVA_COMPUESTA_FRIA[0][1] > CURVA_COMPUESTA_CALIENTE[i][1]){
            suma_inter_serfri[i] = 1;
        }else{
            suma_inter_serfri[i] = 0;
        }
    }
    QVector<double> suma_inter_sercal;
    suma_inter_sercal.resize(CURVA_COMPUESTA_FRIA.size());
    for ( i = 0; i <  CURVA_COMPUESTA_FRIA.size() ; i++){
        if(CURVA_COMPUESTA_FRIA[i][1] > CURVA_COMPUESTA_CALIENTE[CURVA_COMPUESTA_CALIENTE.size()-1][1]){
            suma_inter_sercal[i] = 1;
        }else{
            suma_inter_sercal[i] = 0;
        }
    }
    double SUMA_inter_serfri = std::accumulate(suma_inter_serfri.begin(),suma_inter_serfri.end(),0);
    double SUMA_inter_sercal = std::accumulate(suma_inter_sercal.begin(),suma_inter_sercal.end(),0);
    QVector<QVector<double>> INTERVALOSCALIENTES,INTERVALOSFRIAS;
    nfils = SUMA_inter_sercal + InterpolacionCalientes.size()-1;
    ncols = 2;
    INTERVALOSCALIENTES.resize(nfils);
    for ( i = 0 ; i < nfils ; i++){
        INTERVALOSCALIENTES[i].resize(ncols);
    }
    j=0;
    for ( i = 0 ; i <= nfils ; i++){
        if(i == nfils){
            break;
        }
        if( i < InterpolacionCalientes.size()-1){
            INTERVALOSCALIENTES[i][0] = InterpolacionCalientes[j][0];
            INTERVALOSCALIENTES[i][1] = InterpolacionCalientes[j+1][0];
            j++;
        }else{
            INTERVALOSCALIENTES[i][0] = Calentamiento[0];
            INTERVALOSCALIENTES[i][1] = Calentamiento[1];
        }
    }
    nfils = SUMA_inter_serfri + InterpolacionFrias.size() -1;
    ncols = 2;
    INTERVALOSFRIAS.resize(nfils);
    for ( i = 0 ; i < nfils ; i++){
        INTERVALOSFRIAS[i].resize(ncols);
    }
    j=0;
    for ( i = 0 ; i < nfils ; i++){
        if(i == nfils){
            break;
        }
        if( i < SUMA_inter_serfri){
            INTERVALOSFRIAS[i][0] = Enfriamento[0];
            INTERVALOSFRIAS[i][1] = Enfriamento[1];
        }else{
            INTERVALOSFRIAS[i][0] = InterpolacionFrias[j][0];
            INTERVALOSFRIAS[i][1] = InterpolacionFrias[j+1][0];
            j++;
        }
    }
    QVector<QVector<double>> INTERVALOS_AGRUPADOS;
    nfils = INTERVALOSFRIAS.size();
    ncols = 4;
    INTERVALOS_AGRUPADOS.resize(nfils);
    for ( i = 0; i < nfils ; i++){
        INTERVALOS_AGRUPADOS[i].resize(ncols);
    }
    for ( i = 0; i < nfils ; i++){
        INTERVALOS_AGRUPADOS[i][0] = INTERVALOSCALIENTES[i][0];
        INTERVALOS_AGRUPADOS[i][1] = INTERVALOSCALIENTES[i][1];
        INTERVALOS_AGRUPADOS[i][2] = INTERVALOSFRIAS[i][0];
        INTERVALOS_AGRUPADOS[i][3] = INTERVALOSFRIAS[i][1];
    }

    double Delta2,Delta1;
    QVector<double> DTm;
    r1 = INTERVALOS_AGRUPADOS.size();
    DTm.resize(r1);
    if(CTo==1){
        for(i=0; i < r1; i++){
        Delta2= INTERVALOS_AGRUPADOS[i][3]-INTERVALOS_AGRUPADOS[i][1];
        Delta1= INTERVALOS_AGRUPADOS[i][2]-INTERVALOS_AGRUPADOS[i][0];
        DTm[i] = qFabs((Delta2-Delta1)/(log(Delta2/Delta1)));
        if(DTm[i] != DTm[i]){
            DTm[i] = (qFabs(Delta2) + qFabs(Delta1))/ 2;
        }
        }
    }else if(CCo==1){
        for(i=0; i < r1; i++){
        Delta2= INTERVALOS_AGRUPADOS[i][3]-INTERVALOS_AGRUPADOS[i][1];
        Delta1= INTERVALOS_AGRUPADOS[i][0]-INTERVALOS_AGRUPADOS[i][2];
        DTm[i] = qFabs((Delta2-Delta1)/(log(Delta2/Delta1)));
        if(DTm[i] != DTm[i]){
            DTm[i] = (qFabs(Delta2) + qFabs(Delta1))/ 2;
        }
        }
    }
    QVector<QVector<double>> Calcal;
    r1 = INTERVALOSCALIENTES.size();//INTERVALOS_AGRUPADOS.size();
    r2 = VectorCalientesMATRIZ.size();
    Calcal.resize(r1);
    for( i = 0; i < r1 ; i++){
        Calcal[i].resize(r2);
    }
    for( i = 0; i < r1 ; i++){
        for ( j = 0; j < r2; j++){
            if(INTERVALOSCALIENTES[i][0] < VectorCalientesMATRIZ[j][0]  && INTERVALOSCALIENTES[i][0] > VectorCalientesMATRIZ[j][1]-1){
                Calcal[i][j] = (VectorCalientesMATRIZ[j][2]*(INTERVALOSCALIENTES[i][1]-INTERVALOSCALIENTES[i][0]))
                        /VectorCalientesMATRIZ[j][3];
            }else{
                Calcal[i][j] = 0;
            }
        }
    }
    QVector<QVector<double>> Calfri;
    r1 = INTERVALOSFRIAS.size();//INTERVALOSFRIAS.size();
    r2 = VectorCalientesMATRIZ.size();
    Calfri.resize(r1);
    for( i = 0 ; i < r1; i++){
        Calfri[i].resize(r2);
    }
    for( i = 0; i < r1 ; i++){
        for ( j = 0; j < r2; j++){
            if(INTERVALOSFRIAS[i][0] < VectorFriasMATRIZ[j][1] && INTERVALOSFRIAS[i][0] > VectorFriasMATRIZ[j][0]-1){
                Calfri[i][j] = (VectorFriasMATRIZ[j][2]*(INTERVALOSFRIAS[i][1]-INTERVALOSFRIAS[i][0]))
                        /VectorFriasMATRIZ[j][3];
            }else{
                Calfri[i][j] = 0;
            }
        }
    }
    QVector<double> Entalpia,SUMCalcal,SUMCalfri;
    nfils = Calcal.size();
    SUMCalcal.resize(nfils);
    for( i = 0; i < nfils ; i++){
        SUMCalcal[i] = 0;
        for ( j = 0; j < Calcal[0].size() ; j++)
            SUMCalcal[i] = SUMCalcal[i] + Calcal[i][j];
    }
    nfils = Calfri.size();
    SUMCalfri.resize(nfils);
    for( i = 0; i < nfils ; i++){
        SUMCalfri[i] = 0;
        for ( j = 0; j < Calfri[0].size() ; j++)
            SUMCalfri[i] = SUMCalfri[i] + Calfri[i][j];
    }
    Entalpia.resize(nfils);
    for( i = 0; i < nfils ; i++){
         Entalpia[i] = SUMCalcal[i] + SUMCalfri[i];
    }
    int r1CAL = SUMA_inter_sercal;
    int r2FRI = SUMA_inter_serfri;
    for ( i = 0 ; i < r2FRI ; i++){
        Entalpia[i] = Entalpia[i] + Intervalos_de_Entalpia[i]/Enfriamento[2];
    }
    for ( i = Entalpia.size()-r1CAL; i < Entalpia.size(); i++){
        Entalpia[i] = Entalpia[i] + Intervalos_de_Entalpia[i]/Calentamiento[2];
    }
    //CALCULO DE AREAS;
    QVector<double> Areas;
    Areas.resize(Entalpia.size());
    for ( i = 0 ; i < Entalpia.size(); i++){
        Areas[i] = (Entalpia[i] / DTm[i]); // * 0.092903;
    }
    //AGLOMERADOS DE  AREA ENERGIA
    nfils = Areas.size();
    double  AC = 0, QC = 0;
    MAC = 0, MQC = 0 ;
    for (i = 0; i < r2FRI ; i++){
        AC = AC + Areas[i];
        MAC = MAC + Areas[i];
        QC = QC + Intervalos_de_Entalpia[i];
        MQC = MQC + Intervalos_de_Entalpia[i];

    }
    double AR = 0, QR = 0;
    MAR = 0, MQR = 0;
    for(i = r2FRI; i < nfils-r1CAL; i++){
        AR = AR + Areas[i];
        QR = QR + Intervalos_de_Entalpia[i];
        MAR = MAR + Areas[i];
        MQR = MQR + Intervalos_de_Entalpia[i];
    }
    double AH = 0, QH = 0;
    MAH = 0, MQH = 0;
    for(i = nfils-r1CAL; i < nfils; i++){
        AH = AH + Areas[i];
        QH = QH + Intervalos_de_Entalpia[i];
        MAH = MAH + Areas[i];
        MQH = MQH + Intervalos_de_Entalpia[i];
    }
    nfils = CapitalCost[0].size();
    if(nfils == 5){
        //COSTOS DE CAPITAL ANUALIZADOS
        double Xfactor = pow(1+ CapitalCost[0][3] ,CapitalCost[0][4]);
        double FactorAnual =( CapitalCost[0][3]* Xfactor )/( Xfactor - 1);
        double CapCos1 = (CapitalCost[0][0] + CapitalCost[0][1]* pow(AC,CapitalCost[0][2])) * FactorAnual;
        MCapCos1 = CapCos1;
        Xfactor = pow(1+ CapitalCost[1][3] ,CapitalCost[1][4]);
        FactorAnual =( CapitalCost[1][3]* Xfactor )/( Xfactor - 1);
        double CapCos2 =( CapitalCost[1][0] + CapitalCost[1][1]* pow(AR,CapitalCost[1][2]))* FactorAnual;
        MCapCos2 = CapCos2;
        Xfactor = pow(1+ CapitalCost[2][3] ,CapitalCost[2][4]);
        FactorAnual =( CapitalCost[2][3]* Xfactor )/( Xfactor - 1);
        double CapCos3 =( CapitalCost[2][0] + CapitalCost[2][1]* pow(AH,CapitalCost[2][2]))* FactorAnual;
        MCapCos3 = CapCos3;
        MCostoCapitalTotal = CapCos1 + CapCos2 + CapCos3;
        //COSTOS DE OPERACION
        if(SI == true){
            double OpeCosC =  QC * (24 * 364) * OperationCost[0]; // Calentamiento
            double OpeCosH =  QH * (24 * 364) * OperationCost[1]; // Enfriamiento
            MOpeCosC = OpeCosC;
            MOpeCosH = MOpeCosH;
            MCostoOperacionTotal = OpeCosC + OpeCosH;
        }else if(SIS == true){
            //double OpeCosC =  QC * (24 * 364) * OperationCost[0]; // Calentamiento
            //double OpeCosH =  QH * (24 * 364) * OperationCost[1]; // Enfriamiento
        }
        MCostoTotal = MCostoOperacionTotal + MCostoCapitalTotal ;
    }else if(nfils == 4){
            
    }
}

double Plot_Costos_vs_Areas_Uniforme::getCostoTotal()
{
    return MCostoTotal;
}

double Plot_Costos_vs_Areas_Uniforme::getCostoOperacionTotal()
{
    return MCostoOperacionTotal;
}

double Plot_Costos_vs_Areas_Uniforme::getCostoCapitalTotal()
{
    return MCostoCapitalTotal;
}

double Plot_Costos_vs_Areas_Uniforme::getOpeCosC()
{
    return MOpeCosC;
}

double Plot_Costos_vs_Areas_Uniforme::getOpeCosH()
{
    return MOpeCosH;
}

double Plot_Costos_vs_Areas_Uniforme::getCapCos3()
{
    return MCapCos3;
}

double Plot_Costos_vs_Areas_Uniforme::getCapCos2()
{
    return MCapCos2;
}

double Plot_Costos_vs_Areas_Uniforme::getCapCos1()
{
    return MCapCos1;
}

double Plot_Costos_vs_Areas_Uniforme::getAH()
{
    return MAH;
}

double Plot_Costos_vs_Areas_Uniforme::getAR()
{
    return MAR;
}

double Plot_Costos_vs_Areas_Uniforme::getAC()
{
    return MAC;
}

double Plot_Costos_vs_Areas_Uniforme::getQH()
{
    return MQH;
}

double Plot_Costos_vs_Areas_Uniforme::getQR()
{
    return MQR;
}

double Plot_Costos_vs_Areas_Uniforme::getQC()
{
    return MQC;
}

Plot_Costos_vs_Min_Divera::Plot_Costos_vs_Min_Divera(QVector<double> Tsupply, QVector<double> Ttarget, QVector<double> Cp, QVector<double> h, QVector<double> Calentamiento, QVector<double> Enfriamento, QVector<QVector<double> > CapitalCost, QVector<double> OperationCost, double K, double DTmin, float punto1, float punto2, bool SI, bool SIS)
{
    int i,j,n,nfils,ncols,Pares,contador,k;
    QVector<double> VectorCalientes, VectorCalientesCp;
    QVector<double> VectorFrias,VectorFriasCp, VCCurvas,VFCurvas;
    QVector<double> DeltasTi;
    Plot_curvascompuestasajustadas plot1(Tsupply,Ttarget,Cp,DTmin);
    QVector<QVector<double>> VADHF = plot1.getVecAdjHeatFlow();
    float UTILIDADCALENTAMIENTO = VADHF[0][0];
    float UTILIDADFRIA = VADHF[VADHF.size()-1][1];
    double ERROR;
    float error1,error2,error3,error = 100.00, errorPasado = 100.00,tolerancia = 0.000001;
    QVector<float> ValoresdeK,ValoresdeError;
    ValoresdeK.resize(31);
    ValoresdeError.resize(31);
    i = 0;
    int iterador = 0;
    int min_pos;
    while(error >= tolerancia ){//while(error >= tolerancia ){
        float incremento = (punto2-punto1)/30.0;
        ValoresdeK[0] = double (punto1);
        for(int i = 1; i <= 30; i++){
            ValoresdeK[i] = float (punto1 + incremento);
            punto1 = punto1 + incremento;
        }
        for(int i = 0; i < ValoresdeK.size(); i++){
            K = double (ValoresdeK[i]);
            FindKvalue Ki(Tsupply,Ttarget,Cp,h,K,DTmin);
            float KiUC = Ki.getUC();
            float KiUF = Ki.getUF();
            error1 = qFabs( ((UTILIDADCALENTAMIENTO - KiUC)/KiUC));
            error2 = qFabs( ((UTILIDADFRIA - KiUF )/KiUF ));
            error3 = error1 + error2;
            ValoresdeError[i] = float (error3);
        }
        min_pos = std::distance(ValoresdeError.begin(),std::min_element(ValoresdeError.begin(),ValoresdeError.end()));
        ERROR = *std::min_element(ValoresdeError.begin(),ValoresdeError.end()); //double
        error = float (ERROR);
        if(error == errorPasado){
            break;
        }
        errorPasado = error;
        //qDebug() <<  "error" << error  << "Valor de K" << ValoresdeK[min_pos] << iterador << min_pos;
        if(error > tolerancia){
            punto1 = ValoresdeK[min_pos]-incremento;
            punto2 = ValoresdeK[min_pos]+incremento;
            iterador = iterador +1;
        }
    }
    MK = ValoresdeK[min_pos];
    K = ValoresdeK[min_pos];
    n = Tsupply.size();
    DeltasTi.resize(n);
    Calentamiento[0] = Calentamiento[0] + (DTmin/2 - (K/(Calentamiento[2]/1000)) ) ;
    Calentamiento[1] = Calentamiento[1] + (DTmin/2 - (K/(Calentamiento[2]/1000)) ) ;
    Enfriamento[0] = Enfriamento[0] - (DTmin/2 - (K/(Enfriamento[2]/1000)) ) ;
    Enfriamento[1] = Enfriamento[1] - (DTmin/2 - (K/(Enfriamento[2]/1000)) );
    for (i = 0; i < n; i++){
        DeltasTi[i] = (K / (h[i]/1000));
    }
    for (i = 0; i < n ; ++i)
    {
        if(Tsupply[i] >= Ttarget[i])
        {
            DeltasTi[i] = (K / (h[i]/1000));
            Tsupply[i] = Tsupply[i] + (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] + (DTmin/2 - DeltasTi[i]);
            VectorCalientesCp << Tsupply[i] << Ttarget[i] << Cp[i] << h[i];
            VectorCalientes << Tsupply[i] << Ttarget[i];
        }
        if(Tsupply[i] <= Ttarget[i])
        {
            DeltasTi[i] = (K / (h[i]/1000));
            Tsupply[i] = Tsupply[i] - (DTmin/2 - DeltasTi[i]);
            Ttarget[i] = Ttarget[i] - (DTmin/2 - DeltasTi[i]);
            VectorFriasCp << Tsupply[i] << Ttarget[i] << Cp[i] << h[i];
            VectorFrias << Tsupply[i] << Ttarget[i];
        }
        j++;
    }
    QVector < QVector <double> > VectorCalientesMATRIZ;
    Pares = VectorCalientesCp.size();
    nfils = Pares/4;
    ncols = 4;
    VectorCalientesMATRIZ.resize(nfils);
    for(i = 0; i< VectorCalientesMATRIZ.size(); i++)
    {
        VectorCalientesMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++)
    {
        for(int j = 0; j < VectorCalientesMATRIZ.at(i).size(); j++)
        {
            VectorCalientesMATRIZ[i][j] = VectorCalientesCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VectorFriasMATRIZ;
    Pares = VectorFriasCp.size();
    nfils = Pares/4;
    ncols = 4;
    VectorFriasMATRIZ.resize(nfils);
    for(i = 0; i<  VectorFriasMATRIZ.size(); i++)
    {
        VectorFriasMATRIZ[i].resize(ncols);
    }
    contador = 0;
    for(i = 0; i <  VectorFriasMATRIZ.size(); i++)
    {
        for(j = 0; j <  VectorFriasMATRIZ.at(i).size(); j++)
        {
            VectorFriasMATRIZ[i][j] = VectorFriasCp[contador];
            contador++;
        }
    }
    QVector < QVector <double > > VCC,VFC;
    Pares = VectorCalientes.size();
    nfils = Pares/2;
    ncols = 2;
    VCC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VCC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VCC[i][0] = VectorCalientes[k];
        k++;
        VCC[i][1] = VectorCalientes[k];
        k++;
    }
    Pares = VectorFrias.size();
    nfils = Pares/2;
    ncols = 2;
    VFC.resize(nfils);
    for(i = 0; i <  nfils; i++)
    {
        VFC[i].resize(ncols);
    }
    k=0;
    for(i = 0; i < nfils; i++){
        VFC[i][0] = VectorFrias[k];
        k++;
        VFC[i][1] = VectorFrias[k];
        k++;
    }
    std::sort(VectorCalientes.begin(),VectorCalientes.end());
    std::sort(VectorFrias.begin(),VectorFrias.end());
    VCCurvas = VectorCalientes;
    VFCurvas = VectorFrias;
    QVector<double> VectorCalientesNuevo,VectorFriasNuevo;
    VectorCalientesNuevo.resize(VectorCalientes.size());
    k = VectorCalientes.size() - 1 ;
    for( i = 0; i < VectorCalientes.size(); i++){
        VectorCalientesNuevo[k] = VectorCalientes[i];
        --k;
    }
    k = VectorFrias.size() - 1;
    VectorFriasNuevo.resize(VectorFrias.size());
    for( i = 0; i < VectorFrias.size(); i++){
        VectorFriasNuevo[k] = VectorFrias[i];
        --k;
    }
    n = VCCurvas.size();
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VCCurvas[i] == VCCurvas[j] ){
                    QVector<double> VCCurvaslos;
                    for (i = 0; i < j ; i++){
                        VCCurvaslos << VCCurvas[i];
                    }
                    QVector<double> VCCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VCCurvashi << VCCurvas[j];
                    }
                    VCCurvas.clear();
                    VCCurvas = VCCurvaslos + VCCurvashi;
                    VCCurvaslos.clear();
                    VCCurvashi.clear();
                    i=0;
                    n = VCCurvas.size();
                }
            }
        }
    }
    n = VFCurvas.size();
    //ELIMINAR DUPLICADOS DE CORRIENTES FRIAS
    for (i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i==j){
            }else{
                if (VFCurvas[i] == VFCurvas[j] ){
                    QVector<double> VFCurvaslos;
                    for (i = 0; i < j ; i++){
                        VFCurvaslos << VFCurvas[i];
                    }
                    QVector<double> VFCurvashi;
                    for (j = j + 1 ; j < n ; j++){
                        VFCurvashi << VFCurvas[j];
                    }
                    VFCurvas.clear();
                    VFCurvas = VFCurvaslos + VFCurvashi;
                    VFCurvaslos.clear();
                    VFCurvashi.clear();
                    i=0;
                    n = VFCurvas.size();
                }
            }
        }
    }
    QVector<double> VCCurvasT = VCCurvas;
    QVector<double> VFCurvasT = VFCurvas;
    // SE FORMA LA MATRIZ DE RANGOS CALIENTES
    QVector < QVector < double > > RangosCalientes;
    n = VCCurvas.size();
    nfils = n-1;
    ncols = 3;
    RangosCalientes.resize(nfils);
    for(int i = 0; i< RangosCalientes.size(); i++)
    {
        RangosCalientes[i].resize(ncols);
    }
    for(int i = 0; i < RangosCalientes.size(); i++)
    {
        if(i <= (n-2)){
            RangosCalientes[i][0] = VCCurvas[i] ;
            RangosCalientes[i][1] = VCCurvas[i+1] ;
            RangosCalientes[i][2] = qFabs(VCCurvas[i+1]-VCCurvas[i]);
        }
    }
    // SE FORMA LA MATRIZ DE RANGOS FRIAS
    QVector < QVector < double > > RangosFrias;
    n = VFCurvas.size();
    nfils = n-1,
    ncols = 3;
    RangosFrias.resize(nfils);
    for(int i = 0; i< RangosFrias.size(); i++)
    {
        RangosFrias[i].resize(ncols);
    }
    for(int i = 0; i < RangosFrias.size(); i++)
    {
        if(i <= (n-2)){
            RangosFrias[i][0] = VFCurvas[i] ;
            RangosFrias[i][1] = VFCurvas[i+1] ;
            RangosFrias[i][2] = qFabs(VFCurvas[i+1]-VFCurvas[i]);
        }
    }
    QVector < QVector < double > > sumCpCal;
    n = VCCurvas.size() - 1;;
    nfils = n;
    ncols = VectorCalientesMATRIZ.size(); // tenia 2
    sumCpCal.resize(nfils);
    for(i = 0; i< sumCpCal.size(); i++)
    {
        sumCpCal[i].resize(ncols);
    }
    int l=0;
    int m =VectorCalientesMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k=0;
        for(j = 0; j < m ; j++){
            if(VCCurvas[i]<= VectorCalientesMATRIZ[j][0]-.1 && VCCurvas[i]>= VectorCalientesMATRIZ[j][1]-.1 )
            {
                sumCpCal[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCal[l][k] = 0;
            }
            k++;
        }
        l++;
    }
    // SUMCPFRI
    QVector < QVector < double > > sumCpFri;
    n = VFCurvas.size() - 1;
    nfils = n;
    ncols = VectorFriasMATRIZ.size(); // AQUI DEBE SE VECTORFIRASMATRIZ.SIZE() tenia 2
    sumCpFri.resize(nfils);
    for(i = 0; i< sumCpFri.size(); i++)
    {
        sumCpFri[i].resize(ncols);
    }
    int l1=0;
    m = VectorFriasMATRIZ.size();
    for(i = 0; i < n ; i++)
    {
        int k1=0;
        for(j = 0; j < m ; j++){
            if(VFCurvas[i]>= VectorFriasMATRIZ[j][0]-.1 && VFCurvas[i]<= VectorFriasMATRIZ[j][1]-.1 )
            {
                sumCpFri[l1][k1] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFri[l1][k1] = 0;
            }
            k1++;
        }
        l1++;
    }
    //MODIFICADO
    QVector<double> CpcurvaHot;
    CpcurvaHot.resize(sumCpCal.size());
    n = sumCpCal.size();
    for(i = 0; i < n ; i++){
        CpcurvaHot[i] = 0;
        for(j = 0 ; j < sumCpCal[0].size();j++){
            CpcurvaHot[i] = CpcurvaHot[i] + sumCpCal[i][j];
        }
    }
    QVector<double> CpcurvaCold;
    CpcurvaCold.resize(sumCpFri.size());
    n = sumCpFri.size();
    for(i = 0; i < n ; i++){
        CpcurvaCold[i] = 0;
        for(j = 0 ; j < sumCpFri[0].size();j++){
            CpcurvaCold[i] = CpcurvaCold[i] + sumCpFri[i][j];
        }
    }
    QVector<double> Val1;
    n = CpcurvaCold.size();
    for(i=0; i < n ; i++){
        Val1 << RangosFrias[i][2] * CpcurvaCold[i];
    }
    QVector<double> Val2;
    n = CpcurvaHot.size();
    for(i=0; i < n ; i++){
        Val2 << RangosCalientes[i][2] * CpcurvaHot[i];
    }
    QVector<QVector<double>> PUNTOSCF, PUNTOSCC;
    // PUNTOS CORRIENTES FRIAS
    n = RangosFrias.size();
    nfils = n;
    ncols = 4;
    PUNTOSCF.resize(nfils);
    for(i = 0; i < nfils; i++)
    {
        PUNTOSCF[i].resize(ncols);
    }
    for(i = 0; i < n ; i++)
    {
        PUNTOSCF[i][0] = RangosFrias[i][0];
        PUNTOSCF[i][1] = RangosFrias[i][1];
        PUNTOSCF[i][2] = RangosFrias[i][2];
        PUNTOSCF[i][3] = RangosFrias[i][2]*CpcurvaCold[i];
    }
    // PUNTOS CORRIENTES CALIENNTES
    n = RangosCalientes.size();
    nfils = n;
    ncols = 4;
    PUNTOSCC.resize(nfils);
    for(i = 0; i < nfils; i++){
        PUNTOSCC[i].resize(ncols);
    }
    for(i = 0 ; i < n; i++){
        PUNTOSCC[i][0] = RangosCalientes[i][0];
        PUNTOSCC[i][1] = RangosCalientes[i][1];
        PUNTOSCC[i][2] = RangosCalientes[i][2];
        PUNTOSCC[i][3] = RangosCalientes[i][2]*CpcurvaHot[i];
    }
    // HASTA AQUI ES PARA CURVAS COMPUESTAS
    QVector<QVector<double>> VectorCalientesTmin;
    VectorCalientesTmin.resize(VCCurvasT.size());
    nfils = VCCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i].resize(ncols);
    }
    k = VCCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorCalientesTmin[i][0] = VCCurvasT[k]-DTmin;
        VectorCalientesTmin[i][1] = VCCurvasT[k];
        --k;
    }
    QVector<QVector<double>> VectorFriasTmin;
    VectorFriasTmin.resize(VFCurvasT.size());
    nfils = VFCurvasT.size();
    ncols = 2;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i].resize(ncols);
    }
    k = VFCurvasT.size()-1;
    for(i = 0; i < nfils; i++){
        VectorFriasTmin[i][0] = VFCurvasT[k];
        VectorFriasTmin[i][1] = VFCurvasT[k]+DTmin;
        --k;
    }
    QVector<QVector<double>> VectorCorrientesTotal;
    nfils = VCCurvasT.size() + VFCurvasT.size();
    ncols = 2;
    VectorCorrientesTotal.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotal[i].resize(ncols);
    }
    j=0;
    k=0;
    for(i = 0; i < nfils ; i++){
        if(i < VectorCalientesTmin.size()){
            VectorCorrientesTotal[i][0] = VectorCalientesTmin[j][0];
            VectorCorrientesTotal[i][1] = VectorCalientesTmin[j][1];
            ++j;
        }else{
            VectorCorrientesTotal[i][0] = VectorFriasTmin[k][0];
            VectorCorrientesTotal[i][1] = VectorFriasTmin[k][1];
            ++k;
        }
    }
    std::sort(VectorCorrientesTotal.begin(),VectorCorrientesTotal.end());
    QVector<QVector<double>> VectorCorrientesTotalOrdenar;
    nfils = VectorCorrientesTotal.size();
    ncols = 2;
    VectorCorrientesTotalOrdenar.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i].resize(ncols);
    }
    k = VectorCorrientesTotal.size()-1;
    for(i = 0; i < nfils ; i++){
        VectorCorrientesTotalOrdenar[i][0] = VectorCorrientesTotal[k][0];
        VectorCorrientesTotalOrdenar[i][1] = VectorCorrientesTotal[k][1];
        --k;
    }
    VectorCorrientesTotal = VectorCorrientesTotalOrdenar;
    int r3 = VectorCorrientesTotal.size();
    int r4 = VCC.size();
    //FORMAR LA MATRIZ PARA SUMCPCAL
    QVector<QVector<double>> sumCpCalTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VCC.size();//2;
    sumCpCalTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpCalTab[i].resize(ncols);
    }
    l = 0;
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r4; ++j){
            if(VectorCorrientesTotal[i][1] <= VCC[j][0]-.1 && VectorCorrientesTotal[i][1] >= VCC[j][1] -.1 ){
                sumCpCalTab[l][k] = VectorCalientesMATRIZ[j][2];
            }else{
                sumCpCalTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<QVector<double>> sumCpFriTab;
    nfils = VectorCorrientesTotal.size()-1;
    ncols = VFC.size();//2;
    sumCpFriTab.resize(nfils);
    for( i = 0; i < nfils; i++){
        sumCpFriTab[i].resize(ncols);
    }
    l = 0;
    int r5 = VFC.size();
    for( i = 1; i < r3; ++i){
        k = 0;
        for(j = 0; j < r5; ++j){
            if(VectorCorrientesTotal[i][0] <= VFC[j][1]-.1 && VectorCorrientesTotal[i][0] >= VFC[j][0] -.1){
                sumCpFriTab[l][k] = VectorFriasMATRIZ[j][2];
            }else{
                sumCpFriTab[l][k] = 0.0 ;
            }
            ++k;
        }
        ++l;
    }
    QVector<double> SumatoriaCP,SumatoriaCPCAL,SumatoriaCPFRI;
    nfils = VectorCorrientesTotal.size()-1;
    SumatoriaCP.resize(nfils);
    SumatoriaCPCAL.resize(nfils);
    SumatoriaCPFRI.resize(nfils);
    //MODIFICADO
    for(i = 0; i < nfils ; i++){
        SumatoriaCPCAL[i] = 0;
        for(j = 0 ; j < sumCpCalTab[0].size();j++){
            SumatoriaCPCAL[i] = SumatoriaCPCAL[i] + sumCpCalTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCPFRI[i] = 0;
        for(j = 0 ; j < sumCpFriTab[0].size(); j++){
            SumatoriaCPFRI[i] = SumatoriaCPFRI[i] + sumCpFriTab[i][j];
        }
    }
    for(i = 0; i < nfils ; i++){
        SumatoriaCP[i] = SumatoriaCPFRI[i]- SumatoriaCPCAL[i] ;
    }
    QVector<double> DifTemperaturas;
    nfils = VectorCorrientesTotal.size()-1;
    DifTemperaturas.resize(nfils);
    j = 1;
    for(i = 0; i < VectorCorrientesTotal.size() ; ++i){
        if (j < VectorCorrientesTotal.size()){
            DifTemperaturas[i] = VectorCorrientesTotal[i][1] - VectorCorrientesTotal[j][1];
            ++j;
        }else if(j == VectorCorrientesTotal.size()){
            break;
        }
    }
    //QVector<double> Deficit;
    nfils = DifTemperaturas.size();
    QVector<double> Deficit;
    Deficit.resize(DifTemperaturas.size());
    for( i = 0 ; i < nfils; ++i){
        Deficit[i] = DifTemperaturas[i] * SumatoriaCP[i];
    }
    int Ldef = Deficit.size();
    QVector<QVector<double>> VecHeatFlow;
    QVector<double> VAL1,VAL2;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecHeatFlow.resize(nfils);
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecHeatFlow[i].resize(ncols);
    }
    VAL1[0]=0;
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecHeatFlow[i][0] = VAL1[i]; // Val1
        VecHeatFlow[i][1] = VAL2[i]; // Val2
    }
    double min = *std::min_element(VAL2.begin(),VAL2.end());
    VAL1.clear();
    VAL2.clear();
    VAL1.resize(nfils+1);
    VAL2.resize(nfils);
    QVector<QVector<double>> VecAdjHeatFlow;
    nfils = DifTemperaturas.size();
    ncols = 2;
    VecAdjHeatFlow.resize(nfils);
    for(i = 0; i < nfils ; i++){
        VecAdjHeatFlow[i].resize(ncols);
    }
    VAL1[0]= qFabs(min);
    for(i = 0; i < Ldef ; i++){
        VAL2[i] = VAL1[i] - Deficit[i];
        VAL1[i+1] = VAL2[i];
        VecAdjHeatFlow[i][0] = VAL1[i]; // Val1
        VecAdjHeatFlow[i][1] = VAL2[i]; // Val2
    }
    QVector<double> PuntosCurvasC,PuntosCurvasF;
    n = PUNTOSCC.size();
    PuntosCurvasC.resize(PUNTOSCC.size()+1);
    PuntosCurvasC[0] = 0;
    for(i = 0 ; i < n; i++){
        PuntosCurvasC[i+1] = PuntosCurvasC[i] + PUNTOSCC[i][3];
    }
    n = PUNTOSCF.size();
    PuntosCurvasF.resize(PUNTOSCF.size()+1);
    double max = VecAdjHeatFlow[VecAdjHeatFlow.size()-1][1];
    PuntosCurvasF[0] = max ;
    for(i = 0; i < n; i++){
        PuntosCurvasF[i+1] = PuntosCurvasF[i] + PUNTOSCF[i][3];
    }
    QVector<QVector<double>> CURVA_COMPUESTA_CALIENTE , CCALIENTE;
    QVector<QVector<double>> CURVA_COMPUESTA_FRIA, CCFRIA;
    nfils = PuntosCurvasC.size();
    ncols = 2;
    CURVA_COMPUESTA_CALIENTE.resize(nfils);
    CCALIENTE.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i].resize(ncols);
        CCALIENTE[i].resize(ncols);
    }
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i][0] = VCCurvas[i] - DTmin/2;
        CURVA_COMPUESTA_CALIENTE[i][1] = PuntosCurvasC[i];
        CCALIENTE[i][0] = VCCurvas[i] - DTmin/2;
        CCALIENTE[i][1] = PuntosCurvasC[i];
    }
    nfils = PuntosCurvasF.size();
    ncols = 2;
    CURVA_COMPUESTA_FRIA.resize(nfils);
    CCFRIA.resize(nfils);
    for( i = 0; i < nfils; i++ ){
        CURVA_COMPUESTA_FRIA[i].resize(ncols);
        CCFRIA[i].resize(ncols);
    }
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_FRIA[i][0] = VFCurvas[i] + DTmin/2;
        CURVA_COMPUESTA_FRIA[i][1] = PuntosCurvasF[i];
        CCFRIA[i][0] = VFCurvas[i] + DTmin/2;
        CCFRIA[i][1] = PuntosCurvasF[i];
    }
    QVector<QVector<double>> ACON;
    QVector<double> ACON_ENTALPIA;
    nfils = PuntosCurvasC.size() + PuntosCurvasF.size();
    ncols = 2;
    ACON.resize(nfils);
    ACON_ENTALPIA.resize(nfils);
    for(i = 0 ; i < nfils ; i++){
        ACON[i].resize(ncols);
    }
    j=0;k=0;
    for(i = 0; i < nfils ; i++){
        if(i < PuntosCurvasC.size()){
            ACON[i][0] = VCCurvas[j];
            ACON[i][1] = PuntosCurvasC[j];
            ACON_ENTALPIA[i] = PuntosCurvasC[j];
            j++;
        }else{
            ACON[i][0] = VFCurvas[k];
            ACON[i][1] = PuntosCurvasF[k];
            ACON_ENTALPIA[i] = PuntosCurvasF[k];
            k++;
        }
    }
    std::sort(ACON.begin(),ACON.end());
    std::sort(ACON_ENTALPIA.begin(),ACON_ENTALPIA.end());
    QVector<double> Dif_Intervalos_de_Entalpia;
    QVector<double> Intervalos_de_Entalpia;
    QVector<double> VALOR_INI;
    nfils = ACON_ENTALPIA.size();
    VALOR_INI.resize(nfils);
    Dif_Intervalos_de_Entalpia.resize(nfils-1);
    Intervalos_de_Entalpia.resize(nfils-1);
    VALOR_INI[0] = ACON_ENTALPIA[0];
    j=0;
    for ( i = 0 ; i < nfils-1; ++i){
        Dif_Intervalos_de_Entalpia[i] = qFabs(VALOR_INI[i] - ACON_ENTALPIA[i+1]);
        Intervalos_de_Entalpia[j] = Dif_Intervalos_de_Entalpia[i];
        VALOR_INI[i+1] = std::accumulate(Intervalos_de_Entalpia.begin(),Intervalos_de_Entalpia.end(),0);
        ++j;
    }
    QVector<QVector<double>> InterpolacionCalientes;
    int r1 =  CURVA_COMPUESTA_CALIENTE.size();
    int r2 = CURVA_COMPUESTA_FRIA.size();
    for(i = 0; i < r2 ; i++){
        for(k = 0; k < r1 ; k++){
            if(k==(r1-1)){
                break;
            }
            if(CURVA_COMPUESTA_FRIA[i][1] > CURVA_COMPUESTA_CALIENTE[k][1] &&
                    CURVA_COMPUESTA_FRIA[i][1] < CURVA_COMPUESTA_CALIENTE[k+1][1]){
                    InterpolacionCalientes.resize(1);
                    InterpolacionCalientes[0].resize(2);
                    InterpolacionCalientes[0][0] = CURVA_COMPUESTA_CALIENTE[k][0] +
                            ((CURVA_COMPUESTA_CALIENTE[k+1][0]-CURVA_COMPUESTA_CALIENTE[k][0])
                            /(CURVA_COMPUESTA_CALIENTE[k+1][1]-CURVA_COMPUESTA_CALIENTE[k][1]))*
                            (CURVA_COMPUESTA_FRIA[i][1]-CURVA_COMPUESTA_CALIENTE[k][1]);;
                    InterpolacionCalientes[0][1] = CURVA_COMPUESTA_FRIA[i][1] ;
                    CURVA_COMPUESTA_CALIENTE = CURVA_COMPUESTA_CALIENTE + InterpolacionCalientes;
                    InterpolacionCalientes.clear();
                    k = 0;
                    r1 = CURVA_COMPUESTA_CALIENTE.size();
                    j++;
                    break;
            }
        }
    }
    InterpolacionCalientes.resize(CURVA_COMPUESTA_CALIENTE.size());
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        InterpolacionCalientes[i].resize(2);
    }
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_CALIENTE.at(i).size() ; j++){
            InterpolacionCalientes[i][j] = CURVA_COMPUESTA_CALIENTE[i][j];
        }
    }
    CURVA_COMPUESTA_CALIENTE.clear();
    nfils = CCALIENTE.size();
    ncols = 2;
    CURVA_COMPUESTA_CALIENTE.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_CALIENTE[i].resize(ncols);
    }
    for( i = 0; i < CURVA_COMPUESTA_CALIENTE.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_CALIENTE.at(i).size() ; j++){
             CURVA_COMPUESTA_CALIENTE[i][j] = CCALIENTE[i][j];
        }
    }
    std::sort(InterpolacionCalientes.begin(),InterpolacionCalientes.end());
    QVector<QVector<double>> InterpolacionFrias;
    r1 =  CURVA_COMPUESTA_FRIA.size();
    r2 = CURVA_COMPUESTA_CALIENTE.size();
    for(i = 0; i < r2 ; i++){
        for(k = 0; k < r1 ; k++){
            if(k==(r1-1)){
                break;
            }
            if(CURVA_COMPUESTA_CALIENTE[i][1]>CURVA_COMPUESTA_FRIA[k][1]
                    && CURVA_COMPUESTA_CALIENTE[i][1]<CURVA_COMPUESTA_FRIA[k+1][1]){
                InterpolacionFrias.resize(1);
                InterpolacionFrias[0].resize(2);
                InterpolacionFrias[0][0] = CURVA_COMPUESTA_FRIA[k][0]+
                        ((CURVA_COMPUESTA_FRIA[k+1][0]-CURVA_COMPUESTA_FRIA[k][0])
                        /(CURVA_COMPUESTA_FRIA[k+1][1]-CURVA_COMPUESTA_FRIA[k][1]))*
                        (CURVA_COMPUESTA_CALIENTE[i][1]-CURVA_COMPUESTA_FRIA[k][1]);
                InterpolacionFrias[0][1] = CURVA_COMPUESTA_CALIENTE[i][1] ;
                CURVA_COMPUESTA_FRIA = CURVA_COMPUESTA_FRIA + InterpolacionFrias;
                InterpolacionFrias.clear();
                k = 0;
                r1 = CURVA_COMPUESTA_FRIA.size();
                j++;
                break;
            }
        }
    }
    InterpolacionFrias.resize(CURVA_COMPUESTA_FRIA.size());
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        InterpolacionFrias[i].resize(2);
    }
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_FRIA.at(i).size() ; j++){
            InterpolacionFrias[i][j] = CURVA_COMPUESTA_FRIA[i][j];
        }
    }
    CURVA_COMPUESTA_FRIA.clear();
    nfils = CCFRIA.size();
    ncols = 2;
    CURVA_COMPUESTA_FRIA.resize(nfils);
    for(i = 0; i  < nfils; i++){
        CURVA_COMPUESTA_FRIA[i].resize(ncols);
    }
    for( i = 0; i < CURVA_COMPUESTA_FRIA.size(); i++){
        for ( j = 0; j < CURVA_COMPUESTA_FRIA.at(i).size() ; j++){
             CURVA_COMPUESTA_FRIA[i][j] = CCFRIA[i][j];
        }
    }
    std::sort(InterpolacionFrias.begin(),InterpolacionFrias.end());
    QVector<double> suma_inter_serfri;
    suma_inter_serfri.resize(CURVA_COMPUESTA_CALIENTE.size());
    for ( i = 0; i <  CURVA_COMPUESTA_CALIENTE.size() ; i++){
        if(CURVA_COMPUESTA_FRIA[0][1] > CURVA_COMPUESTA_CALIENTE[i][1]){
            suma_inter_serfri[i] = 1;
        }else{
            suma_inter_serfri[i] = 0;
        }
    }
    QVector<double> suma_inter_sercal;
    suma_inter_sercal.resize(CURVA_COMPUESTA_FRIA.size());
    for ( i = 0; i <  CURVA_COMPUESTA_FRIA.size() ; i++){
        if(CURVA_COMPUESTA_FRIA[i][1] > CURVA_COMPUESTA_CALIENTE[CURVA_COMPUESTA_CALIENTE.size()-1][1]){
            suma_inter_sercal[i] = 1;
        }else{
            suma_inter_sercal[i] = 0;
        }
    }
    double SUMA_inter_serfri = std::accumulate(suma_inter_serfri.begin(),suma_inter_serfri.end(),0);
    double SUMA_inter_sercal = std::accumulate(suma_inter_sercal.begin(),suma_inter_sercal.end(),0);
    QVector<QVector<double>> INTERVALOSCALIENTES,INTERVALOSFRIAS;
    nfils = SUMA_inter_sercal + InterpolacionCalientes.size()-1;
    ncols = 2;
    INTERVALOSCALIENTES.resize(nfils);
    for ( i = 0 ; i < nfils ; i++){
        INTERVALOSCALIENTES[i].resize(ncols);
    }
    j=0;
    for ( i = 0 ; i <= nfils ; i++){
        if(i == nfils){
            break;
        }
        if( i < InterpolacionCalientes.size()-1){
            INTERVALOSCALIENTES[i][0] = InterpolacionCalientes[j][0];
            INTERVALOSCALIENTES[i][1] = InterpolacionCalientes[j+1][0];
            j++;
        }else{
            INTERVALOSCALIENTES[i][0] = Calentamiento[0];
            INTERVALOSCALIENTES[i][1] = Calentamiento[1];
        }
    }
    nfils = SUMA_inter_serfri + InterpolacionFrias.size() -1;
    ncols = 2;
    INTERVALOSFRIAS.resize(nfils);
    for ( i = 0 ; i < nfils ; i++){
        INTERVALOSFRIAS[i].resize(ncols);
    }
    j=0;
    for ( i = 0 ; i < nfils ; i++){
        if(i == nfils){
            break;
        }
        if( i < SUMA_inter_serfri){
            INTERVALOSFRIAS[i][0] = Enfriamento[0];
            INTERVALOSFRIAS[i][1] = Enfriamento[1];
        }else{
            INTERVALOSFRIAS[i][0] = InterpolacionFrias[j][0];
            INTERVALOSFRIAS[i][1] = InterpolacionFrias[j+1][0];
            j++;
        }
    }
    QVector<QVector<double>> INTERVALOS_AGRUPADOS;
    nfils = INTERVALOSFRIAS.size();
    ncols = 4;
    INTERVALOS_AGRUPADOS.resize(nfils);
    for ( i = 0; i < nfils ; i++){
        INTERVALOS_AGRUPADOS[i].resize(ncols);
    }
    for ( i = 0; i < nfils ; i++){
        INTERVALOS_AGRUPADOS[i][0] = INTERVALOSCALIENTES[i][0];
        INTERVALOS_AGRUPADOS[i][1] = INTERVALOSCALIENTES[i][1];
        INTERVALOS_AGRUPADOS[i][2] = INTERVALOSFRIAS[i][0];
        INTERVALOS_AGRUPADOS[i][3] = INTERVALOSFRIAS[i][1];
    }
    QVector<QVector<double>> Calcal,CalcalAreas;
    double Comparacion;
    r1 = INTERVALOSCALIENTES.size();//INTERVALOS_AGRUPADOS.size();
    r2 = VectorCalientesMATRIZ.size();
    Calcal.resize(r1);
    CalcalAreas.resize(r1);
    for( i = 0; i < r1 ; i++){
        Calcal[i].resize(r2);
        CalcalAreas[i].resize(r2);
    }
    for( i = 0; i < r1 ; i++){
        for ( j = 0; j < r2; j++){
            Comparacion = (INTERVALOSCALIENTES[i][0] + INTERVALOSCALIENTES[i][1]) / 2;
            if(Comparacion < VectorCalientesMATRIZ[j][0]  && Comparacion > VectorCalientesMATRIZ[j][1]-.1){
                Calcal[i][j] = (VectorCalientesMATRIZ[j][2]*(INTERVALOSCALIENTES[i][1]-INTERVALOSCALIENTES[i][0]))*((K/VectorCalientesMATRIZ[j][3])*1000);
                CalcalAreas[i][j] = (VectorCalientesMATRIZ[j][2]*(INTERVALOSCALIENTES[i][1]-INTERVALOSCALIENTES[i][0]))/VectorCalientesMATRIZ[j][3];
            }else{
                Calcal[i][j] = 0;
                CalcalAreas[i][j] = 0;
            }
        }
    }
    QVector<QVector<double>> Calfri,CalfriAreas;
    r1 = INTERVALOSFRIAS.size();//INTERVALOSFRIAS.size();
    r2 = VectorCalientesMATRIZ.size();
    Calfri.resize(r1);
    CalfriAreas.resize(r1);
    for( i = 0 ; i < r1; i++){
        Calfri[i].resize(r2);
        CalfriAreas[i].resize(r2);
    }
    for( i = 0; i < r1 ; i++){
        for ( j = 0; j < r2; j++){
            Comparacion = (INTERVALOSFRIAS[i][0] + INTERVALOSFRIAS[i][0]) / 2;
            if(Comparacion < VectorFriasMATRIZ[j][1] && Comparacion > VectorFriasMATRIZ[j][0]-.1){
                Calfri[i][j] = (VectorFriasMATRIZ[j][2]*(INTERVALOSFRIAS[i][1]-INTERVALOSFRIAS[i][0]))*((K/VectorFriasMATRIZ[j][3])*1000);
                CalfriAreas[i][j] = (VectorFriasMATRIZ[j][2]*(INTERVALOSFRIAS[i][1]-INTERVALOSFRIAS[i][0]))/VectorFriasMATRIZ[j][3];
            }else{
                Calfri[i][j] = 0;
                CalfriAreas[i][j] = 0;
            }
        }
    }
    QVector<double> SumatorioQDT, SumatorioQDTAREAS,SUMACAL,SUMACALAREAS,SUMAFRI,SUMAFRIAREAS;
    QVector<double> DeltaTM,DeltaTLM;
    SUMACAL.resize(Calcal.size());
    SUMACALAREAS.resize(Calcal.size());
    SUMAFRI.resize(Calfri.size());
    SUMAFRIAREAS.resize(Calfri.size());
    for(i=0; i < Calcal.size(); i++){
        SUMACAL[i] = 0;
        SUMACALAREAS[i] = 0;
        for(j = 0; j < Calcal[0].size(); j++){
            SUMACAL[i] = SUMACAL[i] + Calcal[i][j];
            SUMACALAREAS[i] = SUMACALAREAS[i] + CalcalAreas[i][j];
        }
    }
    for(i=0; i < Calfri.size(); i++){
        SUMAFRI[i] = 0;
        SUMAFRIAREAS[i] =0;
        for(j = 0; j < Calfri[0].size(); j++){
            SUMAFRI[i] = SUMAFRI[i] + Calfri[i][j];
            SUMAFRIAREAS[i] = SUMAFRIAREAS[i] + CalfriAreas[i][j];
        }
    }
    nfils = Calcal.size();
    SumatorioQDT.resize(Intervalos_de_Entalpia.size());
    SumatorioQDTAREAS.resize(Intervalos_de_Entalpia.size());
    DeltaTM.resize(Intervalos_de_Entalpia.size());
    DeltaTLM.resize(Intervalos_de_Entalpia.size());
    QVector<QVector<double>> DeltaTSF;
    r1 = INTERVALOS_AGRUPADOS.size();
    DeltaTSF.resize(r1);
    for( i = 0 ; i < r1; i++){
        DeltaTSF[i].resize(2);
    }
    for(i = 0; i < r1; i++){
        DeltaTSF[i][0] = qFabs(INTERVALOS_AGRUPADOS[i][0] - INTERVALOS_AGRUPADOS[i][2]);
        DeltaTSF[i][1] = qFabs(INTERVALOS_AGRUPADOS[i][1] - INTERVALOS_AGRUPADOS[i][3]);
    }
    for( i = 0; i < nfils ; i++){
        SumatorioQDT[i] = SUMACAL[i] + SUMAFRI[i];
        SumatorioQDTAREAS[i] = SUMACALAREAS[i] + SUMAFRIAREAS[i];
    }
    int r1CAL = SUMA_inter_sercal;
    int r2FRI = SUMA_inter_serfri;
    for ( i = 0 ; i < r2FRI ; i++){
        SumatorioQDT[i] = SumatorioQDT[i] + Intervalos_de_Entalpia[i]*((K/Enfriamento[2])*1000);
        SumatorioQDTAREAS[i] = SumatorioQDTAREAS[i] + Intervalos_de_Entalpia[i]/Enfriamento[2];
    }
    for ( i = SumatorioQDT.size()-r1CAL; i < SumatorioQDT.size(); i++){
        SumatorioQDT[i] = SumatorioQDT[i] + Intervalos_de_Entalpia[i]*((K/Calentamiento[2])*1000);
        SumatorioQDTAREAS[i] = SumatorioQDTAREAS[i] + Intervalos_de_Entalpia[i]/Calentamiento[2];
    }
    for( i = 0; i < nfils ; i++){
        DeltaTM[i] = SumatorioQDT[i]/ Intervalos_de_Entalpia[i];
        DeltaTSF[i][0] = DeltaTSF[i][0] + DeltaTM[i];
        DeltaTSF[i][1] = DeltaTSF[i][1] + DeltaTM[i];
        DeltaTLM[i] =  (DeltaTSF[i][0] + DeltaTSF[i][1])/2;
    }
    //CALCULO DE AREAS;
    QVector<double> Areas;
    Areas.resize(SumatorioQDTAREAS.size());
    for ( i = 0 ; i < SumatorioQDTAREAS.size(); i++){
        Areas[i] = SumatorioQDTAREAS[i] / DeltaTLM[i];
    }
    //AGLOMERADOS DE  AREA ENERGIA
    nfils = Areas.size();
    double  AC = 0, QC = 0;
    MAC = 0, MQC = 0;
    for (i = 0; i < r2FRI ; i++){
        AC = AC + Areas[i];;
        QC = QC + Intervalos_de_Entalpia[i];
        MAC = MAC + Areas[i];;
        MQC = MQC + Intervalos_de_Entalpia[i];
    }
    double AR = 0, QR = 0;
    MAR = 0, MQR = 0;
    for(i = r2FRI; i < nfils-r1CAL; i++){
        AR = AR + Areas[i];
        QR = QR + Intervalos_de_Entalpia[i];
        MAR = MAR + Areas[i];
        MQR = MQR + Intervalos_de_Entalpia[i];
    }
    double AH = 0, QH = 0;
    MAH = 0, MQH = 0;
    for(i = nfils-r1CAL; i < nfils; i++){
        AH = AH + Areas[i];
        QH = QH + Intervalos_de_Entalpia[i];
        MAH = MAH + Areas[i];
        MQH = MQH + Intervalos_de_Entalpia[i];
    }
    nfils = CapitalCost[0].size();
    if(nfils == 5){
        //COSTOS DE CAPITAL ANUALIZADOS
        double Xfactor = pow(1+ CapitalCost[0][3] ,CapitalCost[0][4]);
        double FactorAnual =( CapitalCost[0][3]* Xfactor )/( Xfactor - 1);
        double CapCos1 = (CapitalCost[0][0] + CapitalCost[0][1]* pow(AC,CapitalCost[0][2])) * FactorAnual;
        MCapCos1 = CapCos1;
        Xfactor = pow(1+ CapitalCost[1][3] ,CapitalCost[1][4]);
        FactorAnual =( CapitalCost[1][3]* Xfactor )/( Xfactor - 1);
        double CapCos2 =( CapitalCost[1][0] + CapitalCost[1][1]* pow(AR,CapitalCost[1][2]))* FactorAnual;
        MCapCos2 = CapCos2;
        Xfactor = pow(1+ CapitalCost[2][3] ,CapitalCost[2][4]);
        FactorAnual =( CapitalCost[2][3]* Xfactor )/( Xfactor - 1);
        double CapCos3 =( CapitalCost[2][0] + CapitalCost[2][1]* pow(AH,CapitalCost[2][2]))* FactorAnual;
        MCapCos3 = CapCos3;
        MCostoCapitalTotal = CapCos1 + CapCos2 + CapCos3;
        //COSTOS DE OPERACION
        if(SI == true){
            double OpeCosC =  QC * (24 * 364) * OperationCost[0]; // Calentamiento
            double OpeCosH =  QH * (24 * 364) * OperationCost[1]; // Enfriamiento
            MCostoOperacionTotal = OpeCosC + OpeCosH;
        }else if(SIS == true){
            //double OpeCosC =  QC * (24 * 364) * OperationCost[0]; // Calentamiento
            //double OpeCosH =  QH * (24 * 364) * OperationCost[1]; // Enfriamiento
        }
        MCostoTotal = MCostoOperacionTotal + MCostoCapitalTotal ;
    }
}

double Plot_Costos_vs_Min_Divera::getCostoTotal()
{
    return MCostoTotal;
}

double Plot_Costos_vs_Min_Divera::getCostoOperacionTotal()
{
    return MCostoOperacionTotal;
}

double Plot_Costos_vs_Min_Divera::getCostoCapitalTotal()
{
    return MCostoCapitalTotal;
}

double Plot_Costos_vs_Min_Divera::getK()
{
    return MK;
}

double Plot_Costos_vs_Min_Divera::getOpeCosC()
{
    return MOpeCosC;
}

double Plot_Costos_vs_Min_Divera::getOpeCosH()
{
    return MOpeCosH;
}

double Plot_Costos_vs_Min_Divera::getCapCos3()
{
    return MCapCos3;
}

double Plot_Costos_vs_Min_Divera::getCapCos2()
{
    return MCapCos2;
}

double Plot_Costos_vs_Min_Divera::getCapCos1()
{
    return MCapCos1;
}

double Plot_Costos_vs_Min_Divera::getAH()
{
    return MAH;
}

double Plot_Costos_vs_Min_Divera::getAR()
{
    return MAR;
}

double Plot_Costos_vs_Min_Divera::getAC()
{
    return MAC;
}

double Plot_Costos_vs_Min_Divera::getQH()
{
    return MQH;
}

double Plot_Costos_vs_Min_Divera::getQR()
{
    return MQR;
}

double Plot_Costos_vs_Min_Divera::getQC()
{
    return MQC;
}

