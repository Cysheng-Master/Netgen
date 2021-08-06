#ifdef NG_PYTHON
#ifdef OCCGEOMETRY

#include <../general/ngpython.hpp>
#include <core/python_ngcore.hpp>
#include "../meshing/python_mesh.hpp"

#include <meshing.hpp>
#include <occgeom.hpp>

#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Trsf.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
// #include <XCAFDoc_VisMaterialTool.hxx>
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeSegment.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepGProp.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepLib.hxx>

#include <Geom2d_Curve.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <GCE2d_MakeCircle.hxx>

#if OCC_VERSION_MAJOR>=7 && OCC_VERSION_MINOR>=4
#define OCC_HAVE_DUMP_JSON
#endif





DLL_HEADER void ExportNgOCCBasic(py::module &m) 
{
  cout << "export basics" << endl;
  
  py::class_<gp_Pnt>(m, "gp_Pnt")
    .def(py::init([] (py::tuple pnt)
                  {
                    if (py::len(pnt) != 3)
                      throw Exception("need 3-tuple to create gp_Pnt");
                    
                    return gp_Pnt(py::cast<double>(pnt[0]),
                                  py::cast<double>(pnt[1]),
                                  py::cast<double>(pnt[2]));
                  }))
    .def(py::init([] (double x, double y, double z) {
          return gp_Pnt(x, y, z);
        }))
    .def_property("x", [](gp_Pnt&p) { return p.X(); }, [](gp_Pnt&p,double x) { p.SetX(x); })
    .def_property("y", [](gp_Pnt&p) { return p.Y(); }, [](gp_Pnt&p,double y) { p.SetY(y); })
    .def_property("z", [](gp_Pnt&p) { return p.Z(); }, [](gp_Pnt&p,double z) { p.SetZ(z); })
    .def("__str__", [] (const gp_Pnt & p) {
        stringstream str;
        str << "(" << p.X() << ", " << p.Y() << ", " << p.Z() << ")";
        return str.str();
      })
    // .def(py::self - py::self)
    .def("__sub__", [](gp_Pnt p1, gp_Pnt p2) { return gp_Vec(p1.X()-p2.X(), p1.Y()-p2.Y(), p1.Z()-p2.Z()); })
    .def("__add__", [](gp_Pnt p, gp_Vec v) { return gp_Pnt(p.X()+v.X(), p.Y()+v.Y(), p.Z()+v.Z()); })
    .def("__sub__", [](gp_Pnt p, gp_Vec v) { return gp_Pnt(p.X()-v.X(), p.Y()-v.Y(), p.Z()-v.Z()); })
    ;
  
  py::class_<gp_Vec>(m, "gp_Vec")
    .def(py::init([] (py::tuple vec)
                  {
                    return gp_Vec(py::cast<double>(vec[0]),
                                  py::cast<double>(vec[1]),
                                  py::cast<double>(vec[2]));
                  }))
    .def(py::init([] (double x, double y, double z) {
          return gp_Vec(x, y, z);
        }))
    .def_property("x", [](gp_Vec&p) { return p.X(); }, [](gp_Vec&p,double x) { p.SetX(x); })
    .def_property("y", [](gp_Vec&p) { return p.Y(); }, [](gp_Vec&p,double y) { p.SetY(y); })
    .def_property("z", [](gp_Vec&p) { return p.Z(); }, [](gp_Vec&p,double z) { p.SetZ(z); })
    .def("__str__", [] (const gp_Vec & p) {
        stringstream str;
        str << "(" << p.X() << ", " << p.Y() << ", " << p.Z() << ")";
        return str.str();
      })
    .def("__add__", [](gp_Vec v1, gp_Vec v2) { return v1+v2; }) // return gp_Vec(v1.X()+v2.X(), v1.Y()+v2.Y(), v1.Z()+v2.Z()); })
    .def("__sub__", [](gp_Vec v1, gp_Vec v2) { return v1-v2; }) // gp_Vec(v1.X()-v2.X(), v1.Y()-v2.Y(), v1.Z()-v2.Z()); })
    .def("__rmul__", [](gp_Vec v, double s) { return s*v; }) // gp_Vec(s*v.X(), s*v.Y(), s*v.Z()); })
    .def("__neg__", [](gp_Vec v) { return -v; }) // gp_Vec(-v.X(), -v.Y(), -v.Z()); })    
    ;

  py::class_<gp_Dir>(m, "gp_Dir")
    .def(py::init([] (py::tuple dir)
                  {
                    return gp_Dir(py::cast<double>(dir[0]),
                                  py::cast<double>(dir[1]),
                                  py::cast<double>(dir[2]));
                  }))
    .def(py::init([] (double x, double y, double z) {
          return gp_Dir(x, y, z);
        }))
    .def(py::init<gp_Vec>())
    .def("__str__", [] (const gp_Dir & p) {
        stringstream str;
        str << "(" << p.X() << ", " << p.Y() << ", " << p.Z() << ")";
        return str.str();
      })
    ;
  
  py::class_<gp_Ax1>(m, "gp_Ax1")
    .def(py::init([](gp_Pnt p, gp_Dir d) {
          return gp_Ax1(p,d);
        }))
    ;
  py::class_<gp_Ax2>(m, "gp_Ax2")
    .def(py::init([](gp_Pnt p, gp_Dir d) {
          return gp_Ax2(p,d);
        }))
    .def(py::init([](const gp_Ax3 & ax3) {
          return gp_Ax2(ax3.Ax2());
        }))
    ;

  py::class_<gp_Ax3>(m, "gp_Ax3")
    .def(py::init([](gp_Pnt p, gp_Dir N, gp_Dir Vx) {
          return gp_Ax3(p,N, Vx);
        }), py::arg("p"), py::arg("n"), py::arg("x"))
    .def(py::init<gp_Ax2>())
    .def_property("p", [](gp_Ax3 & ax) { return ax.Location(); }, [](gp_Ax3&ax, gp_Pnt p) { ax.SetLocation(p); })
    ;


  py::class_<gp_Pnt2d>(m, "gp_Pnt2d")
    .def(py::init([] (py::tuple pnt)
                  {
                    if (py::len(pnt) != 2)
                      throw Exception("need 2-tuple to create gp_Pnt2d");
                    return gp_Pnt2d(py::cast<double>(pnt[0]),
                                    py::cast<double>(pnt[1]));
                  }))
    .def(py::init([] (double x, double y) {
          return gp_Pnt2d(x, y);
        }))
    ;
  py::class_<gp_Vec2d>(m, "gp_Vec2d")
    .def(py::init([] (py::tuple vec)
                  {
                    if (py::len(vec) != 2)
                      throw Exception("need 2-tuple to create gp_Vec2d");                    
                    return gp_Vec2d(py::cast<double>(vec[0]),
                                    py::cast<double>(vec[1]));
                  }))
    .def(py::init([] (double x, double y) {
          return gp_Vec2d(x, y);
        }))
    ;

  py::class_<gp_Dir2d>(m, "gp_Dir2d")
    .def(py::init([] (py::tuple dir)
                  {
                    if (py::len(dir) != 2)
                      throw Exception("need 2-tuple to create gp_Dir2d");                    
                    return gp_Dir2d(py::cast<double>(dir[0]),
                                    py::cast<double>(dir[1]));
                  }))
    .def(py::init([] (double x, double y) {
          return gp_Dir2d(x, y);
        }))
    ;

  py::class_<gp_Ax2d>(m, "gp_Ax2d")
    .def(py::init([](gp_Pnt2d p, gp_Dir2d d) {
          return gp_Ax2d(p,d);
        }))
    ;


  
  py::class_<gp_Trsf>(m, "gp_Trsf")
    .def(py::init<>())    
    .def("SetMirror", [] (gp_Trsf & trafo, const gp_Ax1 & ax) { trafo.SetMirror(ax); return trafo; })
    .def_static("Translation", [] (const gp_Vec & v) { gp_Trsf trafo; trafo.SetTranslation(v); return trafo; })
    .def_static("Scale", [] (const gp_Pnt & p, double s) { gp_Trsf trafo; trafo.SetScale(p,s); return trafo; })    
    .def_static("Mirror", [] (const gp_Ax1 & ax) { gp_Trsf trafo; trafo.SetMirror(ax); return trafo; })
    .def_static("Rotation", [] (const gp_Ax1 & ax, double ang) { gp_Trsf trafo; trafo.SetRotation(ax, ang*M_PI/180); return trafo; })
    .def_static("Rotation", [] (const gp_Pnt & p, const gp_Dir & d, double ang)
                { gp_Trsf trafo; trafo.SetRotation(gp_Ax1(p,d), ang*M_PI/180); return trafo; })    
    .def_static("Transformation", [] (const gp_Ax3 & ax) { gp_Trsf trafo; trafo.SetTransformation(ax); return trafo; })
    .def_static("Transformation", [] (const gp_Ax3 & from, const gp_Ax3 to)
                { gp_Trsf trafo; trafo.SetTransformation(from, to); return trafo; })
    .def(py::self * py::self)
    .def("__call__", [] (gp_Trsf & trafo, const TopoDS_Shape & shape) {
        return BRepBuilderAPI_Transform(shape, trafo).Shape();
      })
    .def("__str__", [](gp_Trsf & trafo)
    {
      stringstream str;
      gp_XYZ xyz = trafo.TranslationPart();
      str << xyz.X() << ", " << xyz.Y() << ", " << xyz.Z();
      return str.str();
    })
    ;

  py::class_<TopLoc_Location>(m, "TopLoc_Location")
    .def(py::init<gp_Trsf>())
    .def("Transformation", [](const TopLoc_Location & loc) { return loc.Transformation(); })
    ;

  py::implicitly_convertible<py::tuple, gp_Pnt>();
  py::implicitly_convertible<py::tuple, gp_Vec>();
  py::implicitly_convertible<py::tuple, gp_Dir>();
  py::implicitly_convertible<gp_Vec, gp_Dir>();  
  py::implicitly_convertible<py::tuple, gp_Pnt2d>();  
  py::implicitly_convertible<py::tuple, gp_Vec2d>();  
  py::implicitly_convertible<py::tuple, gp_Dir2d>();


  py::implicitly_convertible<gp_Ax3, gp_Ax2>();

  static gp_Vec ex(1,0,0), ey(0,1,0), ez(0,0,1);
  m.attr("X") = py::cast(&ex);
  m.attr("Y") = py::cast(&ey);
  m.attr("Z") = py::cast(&ez);

}


#endif // OCCGEOMETRY
#endif // NG_PYTHON
 
