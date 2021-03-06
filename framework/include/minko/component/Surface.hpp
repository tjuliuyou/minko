/*
Copyright (c) 2014 Aerys

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "minko/Common.hpp"

#include "minko/component/AbstractComponent.hpp"

namespace minko
{
    namespace component
    {
        class Surface :
            public AbstractComponent
        {
            friend render::DrawCallPool;

        public:
            typedef std::shared_ptr<Surface>                                        Ptr;
            typedef Signal<Ptr, const std::string&, bool>                           TechniqueChangedSignal;
            typedef Signal<Ptr, std::shared_ptr<component::Renderer>, bool>         VisibilityChangedSignal;

        private:
            typedef std::shared_ptr<data::ArrayProvider>                            ArrayProviderPtr;
            typedef std::shared_ptr<scene::Node>                                    NodePtr;
            typedef std::shared_ptr<data::Container>                                ContainerPtr;
            typedef Signal<ContainerPtr, const std::string&>                        PropertyChangedSignal;
            typedef PropertyChangedSignal::Slot                                     PropertyChangedSlot;
            typedef std::shared_ptr<render::Effect>                                 EffectPtr;
            typedef const std::string&                                              StringRef;
            typedef Signal<ArrayProviderPtr, uint>::Slot                            ArrayIndexChangedSlot;


        private:
            std::string                                                             _name;

            std::shared_ptr<geometry::Geometry>                                     _geometry;
            std::shared_ptr<material::Material>                                     _material;
            std::shared_ptr<render::Effect>                                         _effect;
            std::string                                                             _technique;

            bool                                                                    _visible;
            std::unordered_map<std::shared_ptr<component::Renderer>, bool>          _rendererToVisibility;
            std::unordered_map<std::shared_ptr<component::Renderer>, bool>          _rendererToComputedVisibility;

            TechniqueChangedSignal::Ptr                                             _techniqueChanged;
            VisibilityChangedSignal::Ptr                                            _visibilityChanged;
            VisibilityChangedSignal::Ptr                                            _computedVisibilityChanged;

            Signal<AbstractComponent::Ptr, NodePtr>::Slot                           _targetAddedSlot;
            Signal<AbstractComponent::Ptr, NodePtr>::Slot                           _targetRemovedSlot;
            Signal<NodePtr, NodePtr, NodePtr>::Slot                                 _addedSlot;
            Signal<NodePtr, NodePtr, NodePtr>::Slot                                 _removedSlot;

        public:
            static
            Ptr
            create(std::shared_ptr<geometry::Geometry>      geometry,
                   std::shared_ptr<material::Material>      material,
                   std::shared_ptr<render::Effect>          effect)
            {
                return create("", geometry, material, effect, "default");
            }

            static
            Ptr
            create(const std::string&                       name,
                   std::shared_ptr<geometry::Geometry>      geometry,
                   std::shared_ptr<material::Material>      material,
                   std::shared_ptr<render::Effect>          effect,
                   const std::string&                       technique)
            {
                Ptr surface(new Surface(name, geometry, material, effect, technique));

                surface->initialize();

                return surface;
            }

			AbstractComponent::Ptr
			clone(const CloneOption& option);

            ~Surface()
            {
            }

            inline
            const std::string&
            name() const
            {
                return _name;
            }

            inline
            void
            name(const std::string& value)
            {
                _name = value;
            }

            inline
            std::shared_ptr<geometry::Geometry>
            geometry() const
            {
                return _geometry;
            }

            void
            geometry(std::shared_ptr<geometry::Geometry>);

            inline
            std::shared_ptr<material::Material>
            material() const
            {
                return _material;
            }

            inline
            std::shared_ptr<render::Effect>
            effect() const
            {
                return _effect;
            }

            inline
            const std::string&
            technique() const
            {
                return _technique;
            }

            void
            effect(std::shared_ptr<render::Effect>, const std::string& = "default");

            inline
            bool
            visible() const
            {
                return _visible;
            }

            inline
            bool
            visible(std::shared_ptr<component::Renderer> renderer)
            {
                if (_rendererToVisibility.find(renderer) == _rendererToVisibility.end())
                    _rendererToVisibility[renderer] = _visible;

                return _rendererToVisibility[renderer];
            }

            inline
            void
            visible(bool value)
            {
                for (auto& visibility : _rendererToVisibility)
                    visible(visibility.first, value);

                if (_visible != value)
                {
                    _visible = value;
                    _visibilityChanged->execute(std::static_pointer_cast<Surface>(shared_from_this()), nullptr, _visible);
                }
            }

            void
            visible(std::shared_ptr<component::Renderer>, bool value);

            inline
            bool
            computedVisibility(std::shared_ptr<component::Renderer> renderer)
            {
                if (_rendererToComputedVisibility.find(renderer) == _rendererToComputedVisibility.end())
                    _rendererToComputedVisibility[renderer] = true;
                return _rendererToComputedVisibility[renderer];
            }

            void
            computedVisibility(std::shared_ptr<component::Renderer>, bool value);

            inline
            TechniqueChangedSignal::Ptr
            techniqueChanged() const
            {
                return _techniqueChanged;
            }

            inline
            VisibilityChangedSignal::Ptr
            visibilityChanged() const
            {
                return _visibilityChanged;
            }

            inline
            VisibilityChangedSignal::Ptr
            computedVisibilityChanged() const
            {
                return _computedVisibilityChanged;
            }

        private:
            Surface(std::string                                name,
                    std::shared_ptr<geometry::Geometry>        geometry,
                    std::shared_ptr<material::Material>        material,
                    std::shared_ptr<render::Effect>            effect,
                    const std::string&                         technique);

            Surface(const Surface& surface, const CloneOption& option);

            void
            initialize();

            void
            targetAddedHandler(AbstractComponent::Ptr, NodePtr);

            void
            targetRemovedHandler(AbstractComponent::Ptr, NodePtr);

            void
            addedHandler(NodePtr, NodePtr, NodePtr);

            void
            removedHandler(NodePtr, NodePtr, NodePtr);

            void
            setEffectAndTechnique(EffectPtr, const std::string&, bool updateDrawcalls = true);
        };
    }
}
