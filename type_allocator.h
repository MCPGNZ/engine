#pragma once
namespace pk
{
    template <typename Type, typename Allocator>
    class type_allocator
    {
        public:
        Type* allocate();
        void  dealocate(const Type* object);

        bool owns(const Type* object) const;

        private:
        #pragma region Private variables
        Allocator allocator;
        #pragma endregion
    };

    #pragma region Public templates
    template <typename Type, typename Allocator>
    Type* type_allocator<Type, Allocator>::allocate()
    {
        auto result = allocator.allocate(sizeof(Type));
        return result.construct<Type>();
    }

    template <typename Type, typename Allocator>
    void type_allocator<Type, Allocator>::dealocate(const Type* object)
    {
        allocator.deallocate(memory_block{object, sizeof(Type)});
    }

    template <typename Type, typename Allocator>
    bool type_allocator<Type, Allocator>::owns(const Type* object) const
    {
        return allocator.owns(memory_block{object, sizeof(Type)});
    }
    #pragma endregion
}