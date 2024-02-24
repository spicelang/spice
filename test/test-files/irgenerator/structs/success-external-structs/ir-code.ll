; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Vec = type { i32, i1 }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %v = alloca %struct.Vec, align 8
  %v1 = alloca %struct.Vec, align 8
  store i32 0, ptr %result, align 4
  store %struct.Vec { i32 11, i1 false }, ptr %v, align 4
  call void @_ZN3Vec5printEv(ptr noundef nonnull align 4 dereferenceable(8) %v)
  store %struct.Vec zeroinitializer, ptr %v1, align 4
  call void @_ZN3Vec5printEv(ptr noundef nonnull align 4 dereferenceable(8) %v1)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

declare void @_ZN3Vec5printEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
