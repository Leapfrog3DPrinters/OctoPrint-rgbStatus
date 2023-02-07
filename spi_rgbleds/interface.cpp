#include "interface.h"

static struct PyModuleDef spirgbleds = {
  PyModuleDef_HEAD_INIT,
  "spirgbleds",
  NULL,
  -1,
  SpiRgbLedsMethods
};

PyMODINIT_FUNC PyInit_spirgbleds(void) {
  return PyModule_Create(&spirgbleds);
}


PyObject *initialize(PyObject * self, PyObject *args, PyObject *kwargs, char *keywords[])
{
	char *kwlist[] = { (char*)"color", (char*)"transitionsEnabled", (char*)"transitionRefreshInterval", (char*)"transitionTime",  NULL };

	float color[NUM_COLORS] = { 0.0f, 0.0f, 0.0f, 0.0f };

	bool transitionsEnabled = false;
	unsigned int transitionRefreshInterval = 20;
	unsigned int transitionTime = 200;

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(ffff)iii", kwlist,
		&color[0], &color[1], &color[2], &color[3], &transitionsEnabled, &transitionRefreshInterval, &transitionTime))
		return NULL;

	if (handler != NULL)
		delete handler;

	if (transitionsEnabled)
		handler = new RgbLightHandler(color, transitionRefreshInterval, transitionTime);
	else
		handler = new RgbLightHandler(color);

	return Py_BuildValue("O", Py_True);
}

PyObject *start(PyObject *self, PyObject *args)
{
	if(handler == NULL)
		return Py_BuildValue("O", Py_False);

	handler->start();

	return Py_BuildValue("O", Py_True);
}

PyObject *stop(PyObject *self, PyObject *args)
{
	if (handler == NULL)
		return Py_BuildValue("O", Py_False);

	handler->stop();

	return Py_BuildValue("O", Py_True);
}

PyObject *set_constant_color(PyObject *self, PyObject *args, PyObject *kwargs, char *keywords[])
{
	if (handler == NULL)
		return Py_BuildValue("O", Py_False);

	char target = 0;
	float color[NUM_COLORS] = { 0.0f, 0.0f, 0.0f, 0.0f };
	char *kwlist[] = { (char*)"target", (char*)"color", NULL };

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i(ffff)", kwlist,
		&target, &color[0], &color[1], &color[2], &color[3]))
		return NULL;

	RgbLightConstant pattern = RgbLightConstant(color);
	set_pattern(&pattern, target);

	return Py_BuildValue("O", Py_True);
}
PyObject *set_pulsing_color(PyObject *self, PyObject *args, PyObject *kwargs, char *keywords[])
{
	if (handler == NULL)
		return Py_BuildValue("O", Py_False);

	char target = 0;
	float color[NUM_COLORS] = { 0.0f, 0.0f, 0.0f, 0.0f };
	int speed = 0;
	char *kwlist[] = { (char*)"target", (char*)"color", (char*)"speed", NULL };

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i(ffff)i", kwlist,
		&target, &color[0], &color[1], &color[2], &color[3], &speed))
		return NULL;

	RgbLightPulsing pattern = RgbLightPulsing(color, speed);
	set_pattern(&pattern, target);

	return Py_BuildValue("O", Py_True);
}

PyObject *set_onoff_color(PyObject *self, PyObject *args, PyObject *kwargs, char *keywords[])
{
	if (handler == NULL)
		return Py_BuildValue("O", Py_False);

	char target = 0;
	float color[NUM_COLORS] = { 0.0f, 0.0f, 0.0f, 0.0f };
	int speed = 0;
	char *kwlist[] = { (char*)"target", (char*)"color", (char*)"speed", NULL };

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i(ffff)i", kwlist,
		&target, &color[0], &color[1], &color[2], &color[3], &speed))
		return NULL;

	RgbLightOnOff pattern = RgbLightOnOff(color, speed);
	set_pattern(&pattern, target);

	return Py_BuildValue("O", Py_True);
}

void set_pattern(RgbLightPattern * pattern, char target)
{
	if (target == TARGET_LEFT)
		handler->setPatternLeft(pattern);
	else if (target == TARGET_RIGHT)
		handler->setPatternRight(pattern);
	else
		handler->setPatterns(pattern);
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	const float red[4] = { 1.0, 0.0, 0.0, 0.0 };
	const float green[4] = { 0.0, 1.0, 0.0, 0.0 };
	const float blue[4] = { 0.0, 0.0, 1.0, 0.0 };
	const float white[4] = { 0.0, 0.0, 0.0, 1.0 };
	RgbLightConstant * solid1 = new RgbLightConstant(red);
	RgbLightConstant * solid2 = new RgbLightConstant(green);
	RgbLightPulsing * pulsing1 = new RgbLightPulsing(blue, 1000);
	RgbLightHandler * handler = new RgbLightHandler(white, 50, 200);
	handler->start();
	handler->setPatterns(solid1);
	this_thread::sleep_for(chrono::seconds(1));
	handler->setPatterns(solid2);
	this_thread::sleep_for(chrono::seconds(1));
	handler->setPatterns(pulsing1);
	this_thread::sleep_for(chrono::seconds(60));
	handler->stop();
	delete handler;
	delete solid1, solid2;
#endif
	return 0;
}
