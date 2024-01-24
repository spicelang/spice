; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

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
