# GAME ENGINE (ECS)

Entity component system (ECS) is a software architectural pattern mostly used in video game development for the representation of game world objects. An ECS comprises entities composed from components of data, with systems which operate on entities' components.


## Sparse Array

  An entity is a container of components. It is represented by an integer. When an entity is created, it is added to the sparse array of entities. The sparse array is a vector of integers. The index of the vector is the entity id and the value is the index of the entity in the component. When an entity is destroyed, the entity is removed from the sparse array.

  each entity has a list of components and each component is an any which can be equal to a certain value or None.
  <img src="docs/SparseArray.png" alt="SparseArray">


## Component

  <p>A component is a container of data. It is represented by an integer. When a component is created, it is added to the sparse array of components.
  example of a component :</p>

    struct Position {
        float x;
        float y;
    };
  </p>

## Adding component to entity

  <p>When the system is called, it parse the list of entity and add the component to the entity if the component is not already added.</p>
  <p>For example: When I add a velocity component to entity 1, the system parse the list of entity and add the velocity component to the position component.</p>

  <img src="docs/position.png" alt="position">

## System Operation

  <p>Systems are responsible for defining the behavior of entities by operating on their components. The System class in the provided code includes a method display_drawable() that demonstrates how to iterate over entities with a Drawable component and display relevant information.</p>

    void System::display_drawable() {
        auto &draw = reg.getComponent<Drawable>();

        for (auto &i : draw) {
            if (i) {
                std::cout << "Drawable entity: " << i.value().isDrawable << std::endl;
            }
        }
    }


## Main

  <p>The provided main function showcases the usage of the ECS. It creates a registry, adds components (Position, Velocity, Drawable, Controller), adds two entities, associates a Drawable component with the first entity, and displays drawable entities using the system.</p>

    int main() {
        registry r;
        System s(r);
        r.addAllComponents();

        int first_entity = r.addEntity();
        int second_entity = r.addEntity();

        Sparse_array<Drawable> &obj = r.getComponent<Drawable>();
        obj[first_entity] = Drawable(true);
        s.display_drawable();
    }
