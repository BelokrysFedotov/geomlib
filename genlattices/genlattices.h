/*
 * genlattices.h
 *
 *  Created on: 25.11.2012
 *      Author: moric
 */

#ifndef GENLATTICES_H_
#define GENLATTICES_H_

#ifndef REAL
typedef double REAL;
#endif

#define GENLATTICES_FLAG_PRIMITIVE	0x00
#define GENLATTICES_FLAG_FACE_XY	0x01
#define GENLATTICES_FLAG_FACE_XZ	0x02
#define GENLATTICES_FLAG_FACE_YZ	0x04
#define GENLATTICES_FLAG_FACE		0x07
#define GENLATTICES_FLAG_BODY		0x08
#define GENLATTICES_FLAG_BORDER		0x10
#define GENLATTICES_FLAG_INNER		0x20
#define GENLATTICES_FLAG_ALL		0x30

#define GENLATTICES_MASK_BORDER		0x01
#define GENLATTICES_MASK_INNER		0x02
#define GENLATTICES_MASK_FACE		0x04
#define GENLATTICES_MASK_BODY		0x08

void gen_points_box_3d_Coords(int*NCoords,REAL*Coords,char flags,int*CountOfPoints,REAL**Points,int**PointMask);
void gen_points_box_3d_Nh(REAL*MIN,int*N,REAL*h,char flags,int*CountOfPoints,REAL**Points,int**PointMask);
void gen_points_box_3d_h(REAL*MIN,REAL*MAX,REAL*h,char flags,int*CountOfPoints,REAL**Points,int**PointMask);
void gen_points_box_3d_N(REAL*MIN,REAL*MAX,int*N,char flags,int*CountOfPoints,REAL**Points,int**PointMask);

#endif /* GENLATTICES_H_ */
