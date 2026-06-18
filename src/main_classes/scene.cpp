#include "suph/pch.h"
#include "main_classes/scene.h"

void Scene::mergeScene() {
    try {
        Mesh merged_mesh;

        for (auto& obj : objects) {
            if (obj->isEditeble()) {
                Mesh* mesh = dynamic_cast<Mesh*>(obj.get());
                Mesh append_mesh(*mesh);
                append_mesh.setTransformedVertices(
                    obj->position,
                    obj->forward,
                    obj->right,
                    obj->up
                );
                merged_mesh += append_mesh;
            }
        }

        addObject(std::make_unique<Model>(merged_mesh));
    } catch (const std::exception& e) {
        std::cerr << "Exception in Scene::mergeScene: "
                  << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "Unknown exception in Scene::mergeScene"
                  << std::endl;
        throw;
    }
}
    
void Scene::saveScene()
{
    std::ofstream out("./data/scene.bin", std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Cannot open scene.bin for writing");
    }

    auto writePod = [&](const auto& value) {
        out.write(reinterpret_cast<const char*>(&value), sizeof(value));
        if (!out) throw std::runtime_error("Write error");
    };

    auto writeVertex = [&](const Vertex& v) {
        writePod(v.x);
        writePod(v.y);
        writePod(v.z);
    };

    auto writeMesh = [&](Mesh* mesh) {
        auto& verts = mesh->getVertecies();
        auto& faces = mesh->getFaces();

        std::uint32_t vcount = static_cast<std::uint32_t>(verts.Size());
        std::uint32_t fcount = static_cast<std::uint32_t>(faces.Size());

        writePod(vcount);
        for (auto& v : verts) {
            writeVertex(v);
        }

        writePod(fcount);
        for (auto& f : faces) {
            writePod(f.f1);
            writePod(f.f2);
            writePod(f.f3);
        }
    };

    std::uint32_t count = static_cast<std::uint32_t>(objects.size());
    writePod(count);

    for (auto& ptr : objects) {
        Type type = Type::Object;

        if (dynamic_cast<Camera*>(ptr.get()))
            type = Type::Camera;
        else if (dynamic_cast<LightSource*>(ptr.get()))
            type = Type::LightSource;
        else if (dynamic_cast<Model*>(ptr.get()))
            type = Type::Model;
        else if (dynamic_cast<Ground*>(ptr.get()))
            type = Type::Ground;
        else if (dynamic_cast<Cube*>(ptr.get()))
            type = Type::Cube;
        else if (dynamic_cast<Sphere*>(ptr.get()))
            type = Type::Sphere;
        else if (dynamic_cast<Pyramid*>(ptr.get()))
            type = Type::Pyramid;

        writePod(type);

        writeVertex(ptr->position);
        writeVertex(ptr->forward);
        writeVertex(ptr->right);
        writeVertex(ptr->up);
        writePod(ptr->color);

        switch (type) {
        case Type::Camera: {
            auto* cam = dynamic_cast<Camera*>(ptr.get());
            bool active = cam->getCamActiveState();
            int w = cam->getWidth();
            int h = cam->getHeight();

            writePod(active);
            writePod(w);
            writePod(h);
            break;
        }

        case Type::LightSource: {
            auto* ls = dynamic_cast<LightSource*>(ptr.get());
            bool active = ls->getLSActiveState();
            writePod(active);
            break;
        }

        case Type::Model:
        case Type::Ground:
        case Type::Cube:
        case Type::Sphere:
        case Type::Pyramid: {
            auto* meshObj = dynamic_cast<Mesh*>(ptr.get());
            if (!meshObj) {
                throw std::runtime_error("Mesh expected");
            }
            writeMesh(meshObj);
            break;
        }

        case Type::Object:
        default:
            break;
        }
    }
}

void Scene::rollbackScene()
{
    std::ifstream in("./data/scene.bin", std::ios::binary);
    if (!in.is_open()) {
        throw std::runtime_error("Cannot open scene.bin for reading");
    }

    auto readPod = [&](auto& value) {
        in.read(reinterpret_cast<char*>(&value), sizeof(value));
        if (!in) throw std::runtime_error("Read error");
    };

    auto readVertex = [&]() -> Vertex {
        Vertex v;
        readPod(v.x);
        readPod(v.y);
        readPod(v.z);
        return v;
    };

    auto readMesh = [&]() -> Mesh {
        std::uint32_t vcount = 0;
        std::uint32_t fcount = 0;

        readPod(vcount);

        std::vector<float> verts;
        verts.reserve(static_cast<size_t>(vcount) * 3);

        for (std::uint32_t i = 0; i < vcount; ++i) {
            Vertex v = readVertex();
            verts.insert(verts.end(), { v.x, v.y, v.z });
        }

        readPod(fcount);

        std::vector<int> faces;
        faces.reserve(static_cast<size_t>(fcount) * 3);

        for (std::uint32_t i = 0; i < fcount; ++i) {
            int a, b, c;
            readPod(a);
            readPod(b);
            readPod(c);
            faces.insert(faces.end(), { a, b, c });
        }

        return Mesh(verts, faces);
    };

    std::uint32_t count = 0;
    readPod(count);

    objects.clear();
    camera = nullptr;
    light_src = nullptr;
    objects.reserve(count);

    for (std::uint32_t i = 0; i < count; ++i) {
        Type type;
        readPod(type);

        Vertex pos = readVertex();
        Vertex fwd = readVertex();
        Vertex rgt = readVertex();
        Vertex up  = readVertex();
        Uint32 color;
        readPod(color);

        std::unique_ptr<Object> obj;

        switch (type) {
        case Type::Camera: {
            bool active = false;
            int w = 0, h = 0;
            readPod(active);
            readPod(w);
            readPod(h);

            auto cam = std::make_unique<Camera>(w, h, active);
            cam->position = pos;
            cam->forward  = fwd;
            cam->right    = rgt;
            cam->up       = up;
            cam->color    = color;

            camera = cam.get();
            obj = std::move(cam);
            break;
        }

        case Type::LightSource: {
            bool active = false;
            readPod(active);

            auto ls = std::make_unique<LightSource>(0, 0, active);
            ls->position = pos;
            ls->forward  = fwd;
            ls->right    = rgt;
            ls->up       = up;
            ls->color    = color;

            light_src = ls.get();
            obj = std::move(ls);
            break;
        }

        case Type::Model: {
            Mesh mesh = readMesh();
            auto m = std::make_unique<Model>(Mesh{});
            static_cast<Mesh&>(*m) = std::move(mesh);

            m->position = pos;
            m->forward  = fwd;
            m->right    = rgt;
            m->up       = up;
            m->color    = color;

            obj = std::move(m);
            break;
        }

        case Type::Ground: {
            Mesh mesh = readMesh();
            auto g = std::make_unique<Ground>();
            static_cast<Mesh&>(*g) = std::move(mesh);

            g->position = pos;
            g->forward  = fwd;
            g->right    = rgt;
            g->up       = up;
            g->color    = color;

            obj = std::move(g);
            break;
        }

        case Type::Cube: {
            Mesh mesh = readMesh();
            auto c = std::make_unique<Cube>();
            static_cast<Mesh&>(*c) = std::move(mesh);

            c->position = pos;
            c->forward  = fwd;
            c->right    = rgt;
            c->up       = up;
            c->color    = color;

            obj = std::move(c);
            break;
        }

        case Type::Sphere: {
            Mesh mesh = readMesh();
            auto s = std::make_unique<Sphere>();
            static_cast<Mesh&>(*s) = std::move(mesh);

            s->position = pos;
            s->forward  = fwd;
            s->right    = rgt;
            s->up       = up;
            s->color    = color;

            obj = std::move(s);
            break;
        }

        case Type::Pyramid: {
            Mesh mesh = readMesh();
            auto p = std::make_unique<Pyramid>();
            static_cast<Mesh&>(*p) = std::move(mesh);

            p->position = pos;
            p->forward  = fwd;
            p->right    = rgt;
            p->up       = up;
            p->color    = color;

            obj = std::move(p);
            break;
        }

        case Type::Object:
        default: {
            auto o = std::make_unique<Object>(pos, true, true, false);
            o->forward = fwd;
            o->right   = rgt;
            o->up      = up;
            o->color   = color;
            obj = std::move(o);
            break;
        }
        }

        objects.push_back(std::move(obj));
    }
}
