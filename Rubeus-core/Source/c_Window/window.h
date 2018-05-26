/**
 * @file	Source\c_Window\window.h.
 *
 * @brief	Declares the window class and error callbacks for GLFW
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../c_logger/logger.h"

void getGLFWErrorLog(int error, const char *description);

/**
 * @class	RWindowComponent
 *
 * @brief	A window component that bridges GLFW function calls to handle Rubeus game window.
 *
 * @author	Twarit
 * @date	26-05-2018
 */
class RWindowComponent
{
private:
	/** @brief	GLFW Window object */
	GLFWwindow * m_Window;

	/** @brief	The width of the window */
	int m_Width;

	/** @brief	The height of the window */
	int m_Height;

	/** @brief	The title of the window */
	const char * m_Title;

	/**
	 * @fn	bool RWindowComponent::initWindow(const char *title, int width, int height);
	 *
	 * @brief	Initializes the Rubeus game window
	 *
	 * @author	Twarit
	 * @date	26-05-2018
	 *
	 * @param	title 	The title of the window to be formed.
	 * @param	width 	The width of the window to be formed.
	 * @param	height	The height of the window to be formed.
	 *
	 * @return	True if it succeeds, false if it fails.
	 */
	bool initWindow(const char *title, int width, int height);

public:

	/**
	 * @fn	RWindowComponent::RWindowComponent(const char *title, int width, int height);
	 *
	 * @brief	Constructor
	 *
	 * @author	Twarit
	 * @date	26-05-2018
	 *
	 * @param	title 	The title of the window to be formed.
	 * @param	width 	The width of the window to be formed.
	 * @param	height	The height of the window to be formed.
	 */
	RWindowComponent(const char *title, int width, int height);

	/**
	 * @fn	RWindowComponent::~RWindowComponent();
	 *
	 * @brief	Destructor
	 *
	 * @author	Twarit
	 * @date	26-05-2018
	 */
	~RWindowComponent();

	/**
	 * @fn	void RWindowComponent::clearWindow();
	 *
	 * @brief	Clears the window with a color buffer
	 *
	 * @author	Twarit
	 * @date	26-05-2018
	 */
	void clearWindow();

	/**
	 * @fn	bool RWindowComponent::closed();
	 *
	 * @brief	Queries if the window is closed
	 *
	 * @author	Twarit
	 * @date	26-05-2018
	 *
	 * @return	m_Closed.
	 */
	bool closed();

	/**
	 * @fn	virtual friend static void RWindowComponent::windowCloseCallback(GLFWwindow * window);
	 *
	 * @brief	User definable callback, called when the game window closes. Create a
	 * 			class deriving from RWindowComponent that overrides this function
	 *
	 * @author	Twarit
	 * @date	26-05-2018
	 *
	 * @param [in]	window	The pointer to the window that just got closed.
	 */
	friend void windowCloseCallback(GLFWwindow * window);

protected:
};
