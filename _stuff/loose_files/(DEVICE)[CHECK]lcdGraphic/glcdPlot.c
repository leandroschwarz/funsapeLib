/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			glcdPlot.c
 * Module:			Plot window for Graphic LCD
 * Author:			Leandro Schwarz
 * Version:			1.0
 * Last edition:	2016-05-23
 * Purpose:			Create and controls a plot window on a graphic LCD. This
 *					window can be used to plot a graph.
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "glcdPlot.h"
#if __GLCD_H != 10
	#error Error 101 - Version mismatch on header and source code files (glcdPlot).
#endif

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

// -----------------------------------------------------------------------------
// Static function declarations ------------------------------------------------

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	glcdPlotResetConfiguration
 * Purpose:		Resets the plot window configuration struct
 * Arguments:	plot			Pointer to the plot window struct
 * Returns:		GLCD_PLOT_OK
 * -------------------------------------------------------------------------- */

glcdPlotResult_t glcdPlotResetConfiguration(volatile glcdPlotConfiguration_t * plot)
{
	plot->display = NULL;
	plot->positionX = 0;
	plot->positionY = 0;
	plot->width = 0;
	plot->height = 0;
	plot->counter = 0;
	plot->lastPoint = 0;
	plot->initialized = FALSE;

	return GLCD_PLOT_OK;
}

/* -----------------------------------------------------------------------------
 * Function:	glcdPlotInit
 * Purpose:		Performs the plot window initialization routine
 * Arguments:	plot			Pointer to the plot window struct
 *				display			Pointer to the GLCD configuration struct
 *				x				Origin X coordinate
 *				y				Origin Y coordinate
 *				width			Window width
 *				height			Window height
 * Returns:		GLCD_PLOT_PARAMETER_OUT_OF_RANGE
 *				GLCD_PLOT_OK
 * -------------------------------------------------------------------------- */

glcdPlotResult_t glcdPlotInit(volatile glcdPlotConfiguration_t * plot, volatile glcdConfiguration_t * display, uint16 x, uint8 y, uint16 width, uint8 height)
{
	uint16 i = 0;

	if(width < 1){
		return GLCD_PLOT_PARAMETER_OUT_OF_RANGE;
	}
	if(height < 1){
		return GLCD_PLOT_PARAMETER_OUT_OF_RANGE;
	}

	plot->display = display;
	plot->positionX = x;
	plot->positionY = y;
	plot->width = width;
	plot->height = height;
	plot->counter = 0;
	plot->lastPoint = 0;
	plot->initialized = TRUE;

	// Draws window
	GLCD_Rectangle(display, x, y, (width + x + 2), (height + y + 2), GLCD_COLOR_BLACK);
	for(i = 0;i < width;i++){
		GLCD_Line(display, (i + x + 1), (y + 1), (i + x + 1), (y + height), GLCD_COLOR_WHITE);
	}

	return GLCD_PLOT_OK;
}

/* -----------------------------------------------------------------------------
 * Function:	glcdPlot
 * Purpose:		Adds a point at the plot window
 * Arguments:	plot			Pointer to the plot window struct
 *				point			Point to be plotted
 * Returns:		GLCD_PLOT_PARAMETER_OUT_OF_RANGE
 *				GLCD_PLOT_OK
 * -------------------------------------------------------------------------- */

glcdPlotResult_t glcdPlot(volatile glcdPlotConfiguration_t * plot, uint8 point)
{
	uint16 i;

	if(point > plot->height){
		return GLCD_PLOT_PARAMETER_OUT_OF_RANGE;
	}

// 	for(i = (plot->positionY + 1);i < (plot->positionY + plot->height + 1);i++){
// 		glcdSetPixel(plot->display, (plot->counter + plot->positionX + 1), i, GLCD_COLOR_WHITE);
// 	}
	GLCD_Line(plot->display, (plot->counter + plot->positionX + 1), (plot->positionY + 1), (plot->counter + plot->positionX + 1), (plot->positionY + plot->height), GLCD_COLOR_WHITE);

	if(plot->counter == 0){
		glcdSetPixel(plot->display, (plot->counter + plot->positionX + 1), (plot->positionY + plot->height - point), GLCD_COLOR_BLACK);
	}else{
		GLCD_Line(plot->display, (plot->counter + plot->positionX + 1), (plot->positionY + plot->height - plot->lastPoint), (plot->counter + plot->positionX + 2), (plot->positionY + plot->height - point), GLCD_COLOR_BLACK);
	}

	plot->lastPoint = point;
	plot->counter = ((plot->counter + 1) > plot->width)?0:(plot->counter + 1);

	return GLCD_PLOT_OK;
}





