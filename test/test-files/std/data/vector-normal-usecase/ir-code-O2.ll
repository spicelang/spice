; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__double__Vector__doubleptr_long_long = type { ptr, i64, i64 }

@printf.str.0 = private unnamed_addr constant [17 x i8] c"Vector size: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [21 x i8] c"Vector capacity: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [19 x i8] c"Vector item 5: %f\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %v1 = alloca %__double__Vector__doubleptr_long_long, align 8
  %1 = alloca double, align 8
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  %5 = alloca double, align 8
  %6 = alloca double, align 8
  %7 = alloca double, align 8
  call void @_mp__Vector_double__void__ctor__int(ptr nonnull %v1, i32 3) #2
  store double 1.200000e+00, ptr %1, align 8
  call void @_mp__Vector_double__void__pushBack__doubleref(ptr nonnull %v1, ptr nonnull %1) #2
  store double 0x401DFC5FF5331D64, ptr %2, align 8
  call void @_mp__Vector_double__void__pushBack__doubleref(ptr nonnull %v1, ptr nonnull %2) #2
  store double 5.300000e+00, ptr %3, align 8
  call void @_mp__Vector_double__void__pushBack__doubleref(ptr nonnull %v1, ptr nonnull %3) #2
  store double 0xC10D2BF1D70A3D71, ptr %4, align 8
  call void @_mp__Vector_double__void__pushBack__doubleref(ptr nonnull %v1, ptr nonnull %4) #2
  store double 2.323420e+04, ptr %5, align 8
  call void @_mp__Vector_double__void__pushBack__doubleref(ptr nonnull %v1, ptr nonnull %5) #2
  store double -1.234900e+03, ptr %6, align 8
  call void @_mp__Vector_double__void__pushBack__doubleref(ptr nonnull %v1, ptr nonnull %6) #2
  store double 0.000000e+00, ptr %7, align 8
  call void @_mp__Vector_double__void__pushBack__doubleref(ptr nonnull %v1, ptr nonnull %7) #2
  %8 = call i64 @_mf__Vector_double__long__getSize(ptr nonnull %v1) #2
  %9 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 %8)
  %10 = call i64 @_mf__Vector_double__long__getCapacity(ptr nonnull %v1) #2
  %11 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i64 %10)
  %12 = call ptr @_mf__Vector_double__doubleref__get__int(ptr nonnull %v1, i32 5) #2
  %13 = load double, ptr %12, align 8
  %14 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, double %13)
  call void @_mp__Vector_double__void__dtor(ptr nonnull %v1) #2
  ret i32 0
}

declare void @_mp__Vector_double__void__ctor__int(ptr, i32) local_unnamed_addr

declare void @_mp__Vector_double__void__pushBack__doubleref(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i64 @_mf__Vector_double__long__getSize(ptr) local_unnamed_addr

declare i64 @_mf__Vector_double__long__getCapacity(ptr) local_unnamed_addr

declare ptr @_mf__Vector_double__doubleref__get__int(ptr, i32) local_unnamed_addr

declare void @_mp__Vector_double__void__dtor(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
