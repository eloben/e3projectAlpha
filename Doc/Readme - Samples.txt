Sample application controls

[GraphicsTest]

- Use <Escape> to exit.

[EngineTest]

- Use <Escape> to exit.
- Use <Space> to browse to the next sample.

Sample list:

  1. TriangleSample
	* First sample: renders a simple triangle.
	
  2. VertexFormatSample
	* Tests the mesh model vertex format system. Shaders are pre-selected depending on the mesh vertex format used.
	- The sample renders a triangle and a quad using different vertex formats.
	
  3. MouseCameraSample
	* Tests the framework's object-component model extensibility by implementing a custom handler to move the scene camera.
	- Free movement camera through keyboard and mouse.
	- AWSD keys to translate the camera.
	- Left click and drag to rotate the camera.
	
  4. ChildMeshSample:
	* Tests the framework's object-component model hierarchy by defining a parent-child relationship between a cube (parent) and a sphere (child).
	- Use 1, 2 and 3 to select the X, Y and Z axis.
	- Use 4, 5, 6 to select translation, rotation or scaling operation.
	- Use + and - to increment or decrement the selected operation at the selected axis.
	
  5. HighlightSample:
	* Tests the forward renderer lighting model (normalized Blinn with Schlick's Fresnel term approximation).
	- Controls as in the MouseCameraSample.
	
  6. Dialectrics:
	* Tests the material system rendering dialectric type materials.
	- Controls as in the MouseCameraSample.
	- Use + and - to move the light.
	
  7. Metals:
	* Tests the material system rendering metallic type materials (no environment reflection).
	- Controls as in the MouseCameraSample.
	- Use + and - to move the light.
	
  8. Direct light sample:
	* Tests the dynamic light model defining several dynamic directional lights.
	- Controls as in the MouseCameraSample.
	- Use + and - to rotate a light.
	
  9. Point light sample:
	* Tests the dynamic light model defining several dynamic point lights.
	- Controls as in the MouseCameraSample.
	- Use + and - to scale the lights.
	
  10. Spot light sample:
	* Tests the dynamic light model defining several dynamic spot lights.
	- Controls as in the MouseCameraSample.
	- Use + and - to scale the light cone.
	
  11. Dynamic spot light shadows:
	* Tests the dynamic light model defining several spot lights generating shadows featuring is a preliminar shadow mapping implementation for forward rendering.
	- Controls as in the MouseCameraSample.
	- Use + and - to scale the move the lights.
  12. Dynamic directional light shadows:
	* Tests the dynamic light model defining several directional lights generating shadows featuring a preliminar shadow mapping implementation for forward rendering.
	- Controls as in the MouseCameraSample.

