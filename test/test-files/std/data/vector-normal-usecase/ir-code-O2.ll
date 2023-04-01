; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__double__Vector__doubleptr_long_long_int = type { ptr, i64, i64, i32 }

@printf.str.0 = private unnamed_addr constant [17 x i8] c"Vector size: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [21 x i8] c"Vector capacity: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [19 x i8] c"Vector item 5: %f\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %v1 = alloca %__double__Vector__doubleptr_long_long_int, align 8
  call void @_mp__Vector_double__void__ctor__int(ptr nonnull %v1, i32 3) #2
  call void @_mp__Vector_double__void__pushBack__double(ptr nonnull %v1, double 1.200000e+00) #2
  call void @_mp__Vector_double__void__pushBack__double(ptr nonnull %v1, double 0x401DFC5FF5331D64) #2
  call void @_mp__Vector_double__void__pushBack__double(ptr nonnull %v1, double 5.300000e+00) #2
  call void @_mp__Vector_double__void__pushBack__double(ptr nonnull %v1, double 0xC10D2BF1D70A3D71) #2
  call void @_mp__Vector_double__void__pushBack__double(ptr nonnull %v1, double 2.323420e+04) #2
  call void @_mp__Vector_double__void__pushBack__double(ptr nonnull %v1, double -1.234900e+03) #2
  call void @_mp__Vector_double__void__pushBack__double(ptr nonnull %v1, double 0.000000e+00) #2
  %1 = call i64 @_mf__Vector_double__long__getSize(ptr nonnull %v1) #2
  %2 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 %1)
  %3 = call i64 @_mf__Vector_double__long__getCapacity(ptr nonnull %v1) #2
  %4 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i64 %3)
  %5 = call ptr @_mf__Vector_double__doubleref__get__int(ptr nonnull %v1, i32 5) #2
  %6 = load double, ptr %5, align 8
  %7 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, double %6)
  ret i32 0
}

declare void @_mp__Vector_double__void__ctor__int(ptr, i32) local_unnamed_addr

declare void @_mp__Vector_double__void__pushBack__double(ptr, double) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i64 @_mf__Vector_double__long__getSize(ptr) local_unnamed_addr

declare i64 @_mf__Vector_double__long__getCapacity(ptr) local_unnamed_addr

declare ptr @_mf__Vector_double__doubleref__get__int(ptr, i32) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
