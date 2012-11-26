/*
 * genlattices.c
 *
 *  Created on: 25.11.2012
 *      Author: moric
 */

#include <math.h>
#include <stdlib.h>
#include "genlattices.h"

/**
 * Построить кубическу сетку (облако точек)
 * NCoords - массив int[3] с кол-вом плоскостей по каждой координате
 * Coords - координаты плоскостей;
 * 			Первые NCoords[0] REAL - координаты по оси OX,
 * 			Далее NCoords[1] REAL - коодинаты по оси OY
 * 			Далее NCoords[2] REAL - коодинаты по оси OZ
 * 	flag - флаги построения
 *	CountOfPoints - кол-во точек
 *	Points - точки, по 3 REAL
 *	PointMask - маска точек
 */
void gen_points_box_3d_Coords(int*NCoords,REAL*Coords,char flags,int*OutCountOfPoints,REAL**OutPoints,int**OutPointMask){
	int i,j,k;
	REAL*Points;
	int*PointsMask;
	int CountOfPoints,CurrentPoint;

	for(i=0;i<3;i++)if(NCoords[i]<2)return;

	if(flags==0)flags=GENLATTICES_FLAG_ALL;

#define _CoordsX(i) Coords[i]
#define _CoordsY(i) Coords[NCoords[0]+i]
#define _CoordsZ(i) Coords[NCoords[0]+NCoords[1]+i]

	// Считаем кол-во точек в сетке
	CountOfPoints = 0;
	// внутренние точки сетки
	if(flags&GENLATTICES_FLAG_INNER)
		CountOfPoints+= (NCoords[0]-2)*(NCoords[1]-2)*(NCoords[2]-2);
	// внешние точки сетки
	if(flags&GENLATTICES_FLAG_BORDER)
		CountOfPoints+= NCoords[0]*NCoords[1]*NCoords[2] - (NCoords[0]-2)*(NCoords[1]-2)*(NCoords[2]-2);
	// точки в центрах ячеек
	if(flags&GENLATTICES_FLAG_BODY)
		CountOfPoints+= (NCoords[0]-1)*(NCoords[1]-1)*(NCoords[2]-1);
	// точки в центрах граней XY
	if(flags&GENLATTICES_FLAG_FACE_XY)
		CountOfPoints+= (NCoords[0]-1)*(NCoords[1]-1)*(NCoords[2]);
	// точки в центрах граней XZ
	if(flags&GENLATTICES_FLAG_FACE_XZ)
		CountOfPoints+= (NCoords[0]-1)*(NCoords[1])*(NCoords[2]-1);
	// точки в центрах граней YZ
	if(flags&GENLATTICES_FLAG_FACE_YZ)
		CountOfPoints+= (NCoords[0])*(NCoords[1]-1)*(NCoords[2]-1);

	Points = (REAL*)calloc(CountOfPoints,3*sizeof(REAL));
	PointsMask = (int*)calloc(CountOfPoints,sizeof(int));

	//Заполнение данными
	CurrentPoint = 0;
	// внутренние точки сетки
	if(flags&GENLATTICES_FLAG_INNER){
		for(i=1;i+1<NCoords[0];i++)
			for(j=1;j+1<NCoords[1];j++)
				for(k=1;k+1<NCoords[2];k++){
					Points[CurrentPoint*3+0]=_CoordsX(i);
					Points[CurrentPoint*3+1]=_CoordsY(j);
					Points[CurrentPoint*3+2]=_CoordsZ(k);
					PointsMask[CurrentPoint]=GENLATTICES_MASK_INNER;
					CurrentPoint++;
				}
	}
	// внешние точки сетки
	if(flags&GENLATTICES_FLAG_BORDER){
		for(i=0;i<NCoords[0];i++)
			for(j=0;j<NCoords[1];j++)
				for(k=0;k<NCoords[2];k+=NCoords[2]-1){
					Points[CurrentPoint*3+0]=_CoordsX(i);
					Points[CurrentPoint*3+1]=_CoordsY(j);
					Points[CurrentPoint*3+2]=_CoordsZ(k);
					PointsMask[CurrentPoint]=GENLATTICES_MASK_BORDER;
					CurrentPoint++;
				}
		for(i=0;i<NCoords[0];i++)
			for(j=0;j<NCoords[1];j+=NCoords[1]-1)
				for(k=1;k+1<NCoords[2];k++){
					Points[CurrentPoint*3+0]=_CoordsX(i);
					Points[CurrentPoint*3+1]=_CoordsY(j);
					Points[CurrentPoint*3+2]=_CoordsZ(k);
					PointsMask[CurrentPoint]=GENLATTICES_MASK_BORDER;
					CurrentPoint++;
				}
		for(i=0;i<NCoords[0];i+=NCoords[0]-1)
			for(j=1;j+1<NCoords[1];j++)
				for(k=1;k+1<NCoords[2];k++){
					Points[CurrentPoint*3+0]=_CoordsX(i);
					Points[CurrentPoint*3+1]=_CoordsY(j);
					Points[CurrentPoint*3+2]=_CoordsZ(k);
					PointsMask[CurrentPoint]=GENLATTICES_MASK_BORDER;
					CurrentPoint++;
				}
	}
	// точки в центрах ячеек
	if(flags&GENLATTICES_FLAG_BODY)
		for(i=0;i+1<NCoords[0];i++)
			for(j=0;j+1<NCoords[1];j++)
				for(k=0;k+1<NCoords[2];k++){
					Points[CurrentPoint*3+0]=(_CoordsX(i)+_CoordsX(i+1))/2;
					Points[CurrentPoint*3+1]=(_CoordsY(j)+_CoordsY(j+1))/2;
					Points[CurrentPoint*3+2]=(_CoordsZ(k)+_CoordsZ(k+1))/2;
					PointsMask[CurrentPoint]=GENLATTICES_MASK_BODY;
					CurrentPoint++;
				}
	// точки в центрах граней XY
	if(flags&GENLATTICES_FLAG_FACE_XY)
		for(i=0;i+1<NCoords[0];i++)
			for(j=0;j+1<NCoords[1];j++)
				for(k=0;k<NCoords[2];k++){
					Points[CurrentPoint*3+0]=(_CoordsX(i)+_CoordsX(i+1))/2;
					Points[CurrentPoint*3+1]=(_CoordsY(j)+_CoordsY(j+1))/2;
					Points[CurrentPoint*3+2]=_CoordsZ(k);
					PointsMask[CurrentPoint]=GENLATTICES_MASK_FACE;
					CurrentPoint++;
				}
	// точки в центрах граней XZ
	if(flags&GENLATTICES_FLAG_FACE_XZ)
		for(i=0;i+1<NCoords[0];i++)
			for(j=0;j<NCoords[1];j++)
				for(k=0;k+1<NCoords[2];k++){
					Points[CurrentPoint*3+0]=(_CoordsX(i)+_CoordsX(i+1))/2;
					Points[CurrentPoint*3+1]=_CoordsY(j);
					Points[CurrentPoint*3+2]=(_CoordsZ(k)+_CoordsZ(k+1))/2;
					PointsMask[CurrentPoint]=GENLATTICES_MASK_FACE;
					CurrentPoint++;
				}
	// точки в центрах граней YZ
	if(flags&GENLATTICES_FLAG_FACE_YZ)
		for(i=0;i<NCoords[0];i++)
			for(j=0;j+1<NCoords[1];j++)
				for(k=0;k+1<NCoords[2];k++){
					Points[CurrentPoint*3+0]=_CoordsX(i);
					Points[CurrentPoint*3+1]=(_CoordsY(j)+_CoordsY(j+1))/2;
					Points[CurrentPoint*3+2]=(_CoordsZ(k)+_CoordsZ(k+1))/2;
					PointsMask[CurrentPoint]=GENLATTICES_MASK_FACE;
					CurrentPoint++;
				}

	if(OutCountOfPoints)*OutCountOfPoints = CountOfPoints;
	if(OutPoints){
		*OutPoints=Points;
	}else{
		free(Points);
	}

	if(OutPointMask){
		*OutPointMask=PointsMask;
	}else{
		free(PointsMask);
	}
};

void gen_points_box_3d_Nh(REAL*MIN,int*N,REAL*h,char flags,int*CountOfPoints,REAL**Points,int**PointMask){
	int i,j,n;
	REAL*Coords;
	int NCoords[3];
	for(i=0;i<3;i++)if(N[i]<=0)return;
	for(i=0;i<3;i++)NCoords[i]=N[i]+1;
	Coords = (REAL*)calloc(NCoords[0]+NCoords[1]+NCoords[2],sizeof(REAL));
	for(i=0,n=0;i<3;n+=NCoords[i],i++)
		for(j=0;j<NCoords[i];j++)
			Coords[n+j]=MIN[i]+j*h[i];
	gen_points_box_3d_Coords(NCoords,Coords,flags,CountOfPoints,Points,PointMask);
	free(Coords);
};

void gen_points_box_3d_h(REAL*MIN,REAL*MAX,REAL*h,char flags,int*CountOfPoints,REAL**Points,int**PointMask){
	int i,N[3];
	for(i=0;i<3;i++)N[i]=floor((MAX[i]-MIN[i])/h[i]);
	gen_points_box_3d_Nh(MIN,N,h,flags,CountOfPoints,Points,PointMask);
};

void gen_points_box_3d_N(REAL*MIN,REAL*MAX,int*N,char flags,int*CountOfPoints,REAL**Points,int**PointMask){
	int i;REAL h[3];
	//TODO Возможно на больших числах будет накапливаться ошибка
	for(i=0;i<3;i++)h[i]=(MAX[i]-MIN[i])/N[i];
	gen_points_box_3d_Nh(MIN,N,h,flags,CountOfPoints,Points,PointMask);
}
