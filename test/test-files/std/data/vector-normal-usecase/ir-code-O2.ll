; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__double__Vector__doubleptr_long_long_int = type { ptr, i64, i64, i32 }

@0 = private unnamed_addr constant [17 x i8] c"Vector size: %d\0A\00", align 1
@1 = private unnamed_addr constant [21 x i8] c"Vector capacity: %d\0A\00", align 1
@2 = private unnamed_addr constant [19 x i8] c"Vector item 5: %f\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l3:
  %v1 = alloca %_s__double__Vector__doubleptr_long_long_int, align 8
  call void @_mp__Vector_double__ctor__int(ptr nonnull %v1, i32 3)
  call void @_mp__Vector_double__pushBack__double(ptr nonnull %v1, double 1.200000e+00)
  call void @_mp__Vector_double__pushBack__double(ptr nonnull %v1, double 0x401DFC5FF5331D64)
  call void @_mp__Vector_double__pushBack__double(ptr nonnull %v1, double 5.300000e+00)
  call void @_mp__Vector_double__pushBack__double(ptr nonnull %v1, double 0xC10D2BF1D70A3D71)
  call void @_mp__Vector_double__pushBack__double(ptr nonnull %v1, double 2.323420e+04)
  call void @_mp__Vector_double__pushBack__double(ptr nonnull %v1, double -1.234900e+03)
  call void @_mp__Vector_double__pushBack__double(ptr nonnull %v1, double 0.000000e+00)
  %0 = call i64 @_mf__Vector_double__getSize(ptr nonnull %v1)
  %1 = call i32 (ptr, ...) @printf(ptr nonnull @0, i64 %0)
  %2 = call i64 @_mf__Vector_double__getCapacity(ptr nonnull %v1)
  %3 = call i32 (ptr, ...) @printf(ptr nonnull @1, i64 %2)
  %4 = call double @_mf__Vector_double__get__int(ptr nonnull %v1, i32 5)
  %5 = call i32 (ptr, ...) @printf(ptr nonnull @2, double %4)
  call void @_mp__Vector_double__dtor(ptr nonnull %v1)
  ret i32 0
}

declare void @_mp__Vector_double__ctor__int(ptr, i32) local_unnamed_addr

declare void @_mp__Vector_double__pushBack__double(ptr, double) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare i64 @_mf__Vector_double__getSize(ptr) local_unnamed_addr

declare i64 @_mf__Vector_double__getCapacity(ptr) local_unnamed_addr

declare double @_mf__Vector_double__get__int(ptr, i32) local_unnamed_addr

declare void @_mp__Vector_double__dtor(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
