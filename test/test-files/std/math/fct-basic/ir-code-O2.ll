; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.1 = private unnamed_addr constant [15 x i8] c"Abs (int): %d\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [17 x i8] c"Abs (short): %d\0A\00", align 1
@printf.str.5 = private unnamed_addr constant [16 x i8] c"Abs (long): %d\0A\00", align 1
@printf.str.7 = private unnamed_addr constant [18 x i8] c"Abs (double): %f\0A\00", align 1
@printf.str.10 = private unnamed_addr constant [13 x i8] c"Deg2Rad: %f\0A\00", align 1
@printf.str.13 = private unnamed_addr constant [13 x i8] c"Rad2Deg: %f\0A\00", align 1
@printf.str.14 = private unnamed_addr constant [18 x i8] c"Sin (double): %f\0A\00", align 1
@printf.str.15 = private unnamed_addr constant [15 x i8] c"Sin (int): %f\0A\00", align 1
@printf.str.16 = private unnamed_addr constant [17 x i8] c"Sin (short): %f\0A\00", align 1
@printf.str.17 = private unnamed_addr constant [16 x i8] c"Sin (long): %f\0A\00", align 1
@printf.str.18 = private unnamed_addr constant [18 x i8] c"Cos (double): %f\0A\00", align 1
@printf.str.19 = private unnamed_addr constant [15 x i8] c"Cos (int): %f\0A\00", align 1
@printf.str.20 = private unnamed_addr constant [17 x i8] c"Cos (short): %f\0A\00", align 1
@printf.str.21 = private unnamed_addr constant [16 x i8] c"Cos (long): %f\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call i32 @_f__void__int__abs__int(i32 123) #2
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %1)
  %3 = tail call i32 @_f__void__int__abs__int(i32 -137) #2
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %3)
  %5 = tail call i16 @_f__void__short__abs__short(i16 56) #2
  %6 = sext i16 %5 to i32
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 %6)
  %8 = tail call i16 @_f__void__short__abs__short(i16 -3) #2
  %9 = sext i16 %8 to i32
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 %9)
  %11 = tail call i64 @_f__void__long__abs__long(i64 1234567890) #2
  %12 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i64 %11)
  %13 = tail call i64 @_f__void__long__abs__long(i64 -987654321) #2
  %14 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i64 %13)
  %15 = tail call double @_f__void__double__abs__double(double 5.612300e+01) #2
  %16 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7, double %15)
  %17 = tail call double @_f__void__double__abs__double(double -3.481200e+02) #2
  %18 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7, double %17)
  %19 = tail call double @_f__void__double__degToRad__double(double 4.200000e+02) #2
  %20 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, double %19)
  %21 = tail call double @_f__void__double__degToRad__double(double 4.267800e+01) #2
  %22 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, double %21)
  %23 = tail call double @_f__void__double__degToRad__double(double 3.214530e+02) #2
  %24 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, double %23)
  %25 = tail call double @_f__void__double__radToDeg__double(double 1.000000e+00) #2
  %26 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, double %25)
  %27 = tail call double @_f__void__double__radToDeg__double(double 0.000000e+00) #2
  %28 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, double %27)
  %29 = tail call double @_f__void__double__radToDeg__double(double 0x3FF3C0C9539B8887) #2
  %30 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, double %29)
  %31 = tail call double @_f__void__double__sin__double(double 7.834500e+01) #2
  %32 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.14, double %31)
  %33 = tail call double @_f__void__double__sin__int(i32 23) #2
  %34 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.15, double %33)
  %35 = tail call double @_f__void__double__sin__short(i16 -68) #2
  %36 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, double %35)
  %37 = tail call double @_f__void__double__sin__long(i64 359) #2
  %38 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, double %37)
  %39 = tail call double @_f__void__double__cos__double(double 7.834500e+01) #2
  %40 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.18, double %39)
  %41 = tail call double @_f__void__double__cos__int(i32 23) #2
  %42 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, double %41)
  %43 = tail call double @_f__void__double__cos__short(i16 -68) #2
  %44 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.20, double %43)
  %45 = tail call double @_f__void__double__cos__long(i64 359) #2
  %46 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.21, double %45)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i32 @_f__void__int__abs__int(i32) local_unnamed_addr

declare i16 @_f__void__short__abs__short(i16) local_unnamed_addr

declare i64 @_f__void__long__abs__long(i64) local_unnamed_addr

declare double @_f__void__double__abs__double(double) local_unnamed_addr

declare double @_f__void__double__degToRad__double(double) local_unnamed_addr

declare double @_f__void__double__radToDeg__double(double) local_unnamed_addr

declare double @_f__void__double__sin__double(double) local_unnamed_addr

declare double @_f__void__double__sin__int(i32) local_unnamed_addr

declare double @_f__void__double__sin__short(i16) local_unnamed_addr

declare double @_f__void__double__sin__long(i64) local_unnamed_addr

declare double @_f__void__double__cos__double(double) local_unnamed_addr

declare double @_f__void__double__cos__int(i32) local_unnamed_addr

declare double @_f__void__double__cos__short(i16) local_unnamed_addr

declare double @_f__void__double__cos__long(i64) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
