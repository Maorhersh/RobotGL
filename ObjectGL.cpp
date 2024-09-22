#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "ObjectGL.h"
#include <cmath> // Include for sin() function
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



ObjectGL::ObjectGL(string inputfile, GLfloat PosX, GLfloat PosY, GLfloat PosZ, GLfloat scale,
    glm::vec3 upVector, glm::vec3 towardVector, GLfloat angle) {
    if (!FileExists(inputfile)) {
        // Append default objects dir.
        inputfile = OBJECTS_DIR + "/" + inputfile;
    }
    this->inputfile = inputfile;
    setPosition(PosX, PosY, PosZ);
    this->upVector = upVector;
    this->towardVector = towardVector;
    this->angle = angle;
    this->scale = scale;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    string base_dir = GetBaseDir(inputfile);
    if (base_dir.empty()) {
        base_dir = ".";
    }
#ifdef _WIN32
    base_dir += "\\";
#else
    base_dir += "/";
#endif

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, this->inputfile.c_str(), base_dir.c_str()); // load the .obj file

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "ERROR: " << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    this->attrib = attrib;
    this->shapes = shapes;
    this->materials = materials;

    for (size_t s = 0; s < this->shapes.size(); s++) {
        this->shapesTasks[this->shapes[s].name] = vector<function<void()>>(); // insert empty tasks vectors
    }

    // create textures
    this->textures[""] = 0; // if no given texture don't use texture
    GLuint texture_id;
    string texture_filename;
    // for each material
    for (size_t m = 0; m < materials.size(); m++) {
        tinyobj::material_t* mp = &materials[m];
        texture_filename = mp->diffuse_texname;
        // find texture file
        if (this->textures.find(texture_filename) == this->textures.end()) {
            if (FileExists(base_dir + mp->diffuse_texname)) {
                // Append base dir.
                texture_filename = base_dir + mp->diffuse_texname;
                std::cout << "Texture name: " << texture_filename  << std::endl;

                
            }
            texture_id = create_texture(texture_filename); // create the texture in OpenGL
            this->textures.insert(make_pair(mp->diffuse_texname, texture_id)); // insert the texture id to the textures map
        }
    }
}

void ObjectGL::draw() {
    glPushMatrix();

    glTranslatef(PosX, PosY, PosZ); // Move the object to the desired position
    glRotatef(angle, this->upVector.x, this->upVector.y, this->upVector.z); // Rotate the object
    glScalef(scale, scale, scale); // Scale the object

    // Call all the tasks in the GLOBAL tasks vector
    for (function<void()> task : this->shapesTasks["GLOBAL"]) {
        task();
    }

    // Loop over shapes
    for (size_t s = 0; s < this->shapes.size(); s++) {
        glPushMatrix();

        // Call all the shape's tasks
        for (function<void()> task : this->shapesTasks[this->shapes[s].name]) {
            task();
        }

        // Loop over faces (polygons)
        size_t index_offset = 0;
        for (size_t f = 0; f < this->shapes[s].mesh.num_face_vertices.size(); f++) {
            // Get material
            int current_material_id = this->shapes[s].mesh.material_ids[f];
            if (current_material_id < 0 || current_material_id >= this->materials.size()) {
                // std::cerr << "Material index out of range: " << current_material_id << std::endl;
                current_material_id = -1; // Indicate no material
            }

            // If the material index is valid, set the material properties
            if (current_material_id != -1) {
                tinyobj::material_t* material = &this->materials[current_material_id];

                // Bind texture
                string diffuse_texname = material->diffuse_texname;
                if (this->textures.find(diffuse_texname) == this->textures.end()) {
                    std::cerr << "Texture not found: " << diffuse_texname << std::endl;
                    glBindTexture(GL_TEXTURE_2D, 0); // Bind default texture
                }
                else {
                    glBindTexture(GL_TEXTURE_2D, this->textures[diffuse_texname]);
                }

                // Set material color settings
                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material->diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular);
                glMaterialfv(GL_FRONT, GL_EMISSION, material->emission);
                glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);
            }
            else {
                // Use default material properties if no valid material is assigned
                GLfloat default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
                GLfloat default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
                GLfloat default_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
                GLfloat default_shininess = 0.0f;

                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, default_diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
                glMaterialfv(GL_FRONT, GL_EMISSION, default_emission);
                glMaterialf(GL_FRONT, GL_SHININESS, default_shininess);
                glBindTexture(GL_TEXTURE_2D, 0); // Bind default texture
            }

            int fv = this->shapes[s].mesh.num_face_vertices[f];

            glBegin(GL_POLYGON);

            // Loop over vertices in the face
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = this->shapes[s].mesh.indices[index_offset + v];

                // Get normal
                if (idx.normal_index != -1) {
                    if (3 * idx.normal_index + 2 < this->attrib.normals.size()) {
                        tinyobj::real_t nx = this->attrib.normals[3 * idx.normal_index + 0];
                        tinyobj::real_t ny = this->attrib.normals[3 * idx.normal_index + 1];
                        tinyobj::real_t nz = this->attrib.normals[3 * idx.normal_index + 2];
                        glNormal3f(nx, ny, nz);
                    }
                    else {
                        std::cerr << "Normal index out of range: " << idx.normal_index << std::endl;
                    }
                }

                // Get texture coordinates
                if (idx.texcoord_index != -1) {
                    if (2 * idx.texcoord_index + 1 < this->attrib.texcoords.size()) {
                        tinyobj::real_t tx = this->attrib.texcoords[2 * idx.texcoord_index + 0];
                        tinyobj::real_t ty = this->attrib.texcoords[2 * idx.texcoord_index + 1];
                        glTexCoord2f(tx, ty);
                    }
                    else {
                        std::cerr << "Texcoord index out of range: " << idx.texcoord_index << std::endl;
                    }
                }

                // Get vertex
                if (idx.vertex_index != -1) {
                    if (3 * idx.vertex_index + 2 < this->attrib.vertices.size()) {
                        tinyobj::real_t vx = this->attrib.vertices[3 * idx.vertex_index + 0];
                        tinyobj::real_t vy = this->attrib.vertices[3 * idx.vertex_index + 1];
                        tinyobj::real_t vz = this->attrib.vertices[3 * idx.vertex_index + 2];
                        glVertex3f(vx, vy, vz);
                    }
                    else {
                        std::cerr << "Vertex index out of range: " << idx.vertex_index << std::endl;
                    }
                }
            }
            glEnd();

            index_offset += fv;
        }
        glPopMatrix();
    }

    // Clear texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

// Set the object's position
void ObjectGL::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    this->PosX = x;
    this->PosY = y;
    this->PosZ = z;
}

void ObjectGL::walk(GLfloat distance) {
    float x = this->PosX + distance * this->towardVector.x;
    float y = this->PosY + distance * this->towardVector.y;
    float z = this->PosZ + distance * this->towardVector.z;
    setPosition(x, y, z);
}

void ObjectGL::addTask(function<void()> func, string shape) {
    this->shapesTasks[shape].push_back(func); // push the tasks to the vector
}

void ObjectGL::rotate(GLfloat angle) {
    float rad_angle = (angle / 180) * glm::pi<float>(); // use radians
    glm::mat4 rotationMat(1);
    rotationMat = glm::rotate(rotationMat, rad_angle, this->upVector);
    this->towardVector = glm::vec3(rotationMat * glm::vec4(this->towardVector, 1.0)); // rotate towardVector
    this->angle += angle;
}

GLuint ObjectGL::create_texture(string texture_filename) {
    if (!FileExists(texture_filename)) {
        // Append textures dir.
        texture_filename = TEXTURES_DIR + "/" + texture_filename;
        if (!FileExists(texture_filename)) {
            std::cerr << "Unable to find texture file: " << texture_filename << std::endl;
            exit(1);
        }
    }
    std::cout << "Loading texture: " << texture_filename << std::endl;

    GLuint texture_id;
    int w, h, comp;
    unsigned char* image = stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
    if (!image) {
        std::cerr << "Failed to load texture: " << texture_filename << std::endl;
        exit(1);
    }

    std::cout << "Texture details - Width: " << w << ", Height: " << h << ", Components: " << comp << std::endl;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format = (comp == 3) ? GL_RGB : (comp == 4) ? GL_RGBA : 0;
    if (format == 0) {
        std::cerr << "Unsupported image format: " << comp << " components" << std::endl;
        stbi_image_free(image);
        assert(0);
    }

    if (w % 4 != 0) {
        std::cerr << "Warning: Texture width is not a multiple of 4, which may cause issues with some drivers." << std::endl;
    }
    if (h % 4 != 0) {
        std::cerr << "Warning: Texture height is not a multiple of 4, which may cause issues with some drivers." << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, image);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error after glTexImage2D: " << error << std::endl;
        stbi_image_free(image);
        exit(1);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    std::cout << "Texture loaded successfully: " << texture_id << std::endl;
    return texture_id;
}

bool FileExists(const std::string& abs_filename) {
    ifstream f(abs_filename.c_str());
    return f.good();
}

string GetBaseDir(const std::string& filepath) {
    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";
}

void ObjectGL::setVibration(bool enable, float initialPos) {
    vibrating = enable;
    
    if (enable) {
        initialPos = initialPos; // Store the initial Y position when vibration is enabled
    }
    else {
        PosY = initialPos; // Reset to initial Y position when vibration is disabled
    }
}

void ObjectGL::vibrate(float amplitude, float frequency, float time, float initialPos) {
    float result;
    if (vibrating) {
        result = PosY + amplitude * sin(2 * M_PI * frequency * time);
        if (result >= this->initY) {
            PosY = result;
        }
    }
}