/****************************************************************************
 *
 * $Id$
 *
 * Copyright (C) 1998-2010 Inria. All rights reserved.
 *
 * This software was developed at:
 * IRISA/INRIA Rennes
 * Projet Lagadic
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * http://www.irisa.fr/lagadic
 *
 * This file is part of the ViSP toolkit.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE included in the packaging of this file.
 *
 * Licensees holding valid ViSP Professional Edition licenses may
 * use this file in accordance with the ViSP Commercial License
 * Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 *
 * Description:
 * Use to display an image behind the internal view of the simulator
 * used for augmented reality application
 *
 * Authors:
 * Eric Marchand
 * Fabien Spindler
 *
 *****************************************************************************/

/*!
  \file vpAR.cpp
  \brief class used to display an image behind the internal view of the simulator. Used for augmented reality applications.
*/

#include <visp/vpConfig.h>

#ifdef VISP_HAVE_COIN

#include <visp/vpAR.h>
#include <visp/vpTime.h>


/* Objets OIV. */
#include <Inventor/nodes/SoCone.h> /* Objet cone.                            */
#include <Inventor/nodes/SoCylinder.h> /* Objet cylindre.                    */
#include <Inventor/nodes/SoPointLight.h> /* Objet lumiere ponctuelle.        */
#include <Inventor/nodes/SoCoordinate3.h> /* Liste de points.                */
#include <Inventor/nodes/SoIndexedFaceSet.h> /* Liste de face.               */
#include <Inventor/nodes/SoTranslation.h> /* Trasnfo translation.            */
#include <Inventor/nodes/SoScale.h> /* Trasnfo mise a l'echelle.             */
#include <Inventor/nodes/SoRotationXYZ.h> /* Transfo rotation simple.       */

#include <Inventor/nodes/SoDirectionalLight.h> /* Objet lumiere directionnelle*/
#include <Inventor/nodes/SoMaterial.h> /* Matiere (couleur) des objets.     */
#include <Inventor/nodes/SoDrawStyle.h> /* Style de rendu.                  */
#include <Inventor/nodes/SoEnvironment.h> /* Eclairage ambiant.              */
#include <Inventor/nodes/SoGroup.h> /* Groupement de noeuds (sans separation)*/
#include <Inventor/actions/SoWriteAction.h>



/*!
	Basic Destructor that calls the kill() method of the vpSimulator class.
*/
vpAR::~vpAR()
{
  kill() ;
}


/*!
	Initialisation of the internal view of the simulator.

	\param width : Width of the internal view.
	\param height : Height of the internal view.
	\param type : Type of background image ie gray scaled or color.
*/
void
vpAR::initInternalViewer(int width, int height,  vpImageType type)
{

  vpSimulator::initInternalViewer(width,height) ;


  // no image is loaded
  background = false ;

  if ( image_background != NULL)
  {
    free(image_background) ;
    image_background = NULL ;
  }

  typeImage = type;
  if (typeImage == grayImage)
    image_background =(GLubyte *)
      malloc(internal_width*internal_height*sizeof(GLubyte)) ;
  else
    image_background =(GLubyte *)
      malloc(3*internal_width*internal_height*sizeof(GLubyte)) ;

}



/*!
	Set the background image and turn it to deal with the frame of OpenGL.

	\param I : Gray scaled image for the background.
*/
// Grey pictures SetBackGroundImage
void
vpAR::setImage(vpImage<unsigned char> &I)
{

  if ((internal_width != (int)I.getWidth()) ||
      (internal_height != (int)I.getHeight()))
	{
	  vpERROR_TRACE("The image size is different from the view size ");
	  throw(vpException(vpException::dimensionError),"The image size is different from the view size") ;
	}


  background = true ;


  for (unsigned int i=0 ; i < I.getHeight() ; i++)
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
      //le repere image open GL est en bas a gauche donc l'image serait inverse
      image_background[i*I.getWidth()+j] = I[I.getHeight()-i-1][j] ;

}



/*!
	Set the background image and turn it to deal with the frame of OpenGL.

	\param I : Color image for the background.
*/
// Color pictures SetBackGroundImage
void
vpAR::setImage(vpImage<vpRGBa> &I)
{

  if ((internal_width != (int)I.getWidth()) ||
      (internal_height != (int)I.getHeight()))
	{
	  vpERROR_TRACE("The image size is different from the view size ");
	  throw(vpException(vpException::dimensionError),"The image size is different from the view size") ;
	}


  background = true ;

  unsigned int k =0 ;
  for (unsigned int i=0 ; i <I.getHeight()  ; i++)
    {
      k=0;
      for (unsigned int j=0 ; j <I.getWidth()   ; j++)
	//le repere image open GL est en bas a gauche donc l'image serait inverse
	{
	  image_background[i*I.getWidth()*3+k+0]=I[I.getHeight()-i-1][j].R ;
	  image_background[i*I.getWidth()*3+k+1]=I[I.getHeight()-i-1][j].G ;
	  image_background[i*I.getWidth()*3+k+2]=I[I.getHeight()-i-1][j].B ;
	  k+=3;
      }
    }

}

#endif

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
