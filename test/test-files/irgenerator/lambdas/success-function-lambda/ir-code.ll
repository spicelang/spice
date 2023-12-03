; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.String = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [18 x i8] c"Callback called!\0A\00", align 1
@printf.str.0 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@printf.str.1 = private unnamed_addr constant [35 x i8] c"Callback called with args: %s, %f\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@anon.string.2 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.3 = private unnamed_addr constant [35 x i8] c"Callback called with args: %s, %d\0A\00", align 1
@printf.str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@anon.string.3 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %fat.ptr = alloca { ptr, ptr }, align 8
  %callbackWithoutArgs = alloca { ptr, ptr }, align 8
  %fat.ptr1 = alloca { ptr, ptr }, align 8
  %callbackWithArgs1 = alloca { ptr, ptr }, align 8
  %1 = alloca %struct.String, align 8
  %fat.ptr3 = alloca { ptr, ptr }, align 8
  %callbackWithArgs2 = alloca { ptr, ptr }, align 8
  %2 = alloca %struct.String, align 8
  store i32 0, ptr %result, align 4
  %3 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 0
  store ptr @_Z14lambda.L2C39.0v, ptr %3, align 8
  %4 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %4, ptr %callbackWithoutArgs, align 8
  %5 = getelementptr inbounds { ptr, ptr }, ptr %callbackWithoutArgs, i32 0, i32 0
  %fct = load ptr, ptr %5, align 8
  %6 = call ptr %fct()
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %6)
  %8 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr1, i32 0, i32 0
  store ptr @_Z14lambda.L7C50.0R6Stringd, ptr %8, align 8
  %9 = load { ptr, ptr }, ptr %fat.ptr1, align 8
  store { ptr, ptr } %9, ptr %callbackWithArgs1, align 8
  %10 = getelementptr inbounds { ptr, ptr }, ptr %callbackWithArgs1, i32 0, i32 0
  call void @_ZN6String4ctorEPc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr @anon.string.2)
  %fct2 = load ptr, ptr %10, align 8
  %11 = call i1 %fct2(ptr %1, double 3.140000e+00)
  %12 = zext i1 %11 to i32
  %13 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %12)
  %14 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr3, i32 0, i32 0
  store ptr @_Z15lambda.L13C49.06Strings, ptr %14, align 8
  %15 = load { ptr, ptr }, ptr %fat.ptr3, align 8
  store { ptr, ptr } %15, ptr %callbackWithArgs2, align 8
  %16 = getelementptr inbounds { ptr, ptr }, ptr %callbackWithArgs2, i32 0, i32 0
  call void @_ZN6String4ctorEPc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr @anon.string.3)
  %17 = load %struct.String, ptr %2, align 8
  %fct4 = load ptr, ptr %16, align 8
  %18 = call i16 %fct4(%struct.String %17, i16 321)
  %19 = xor i16 %18, 956
  %20 = sext i16 %19 to i32
  %21 = icmp eq i32 %20, 1
  %22 = select i1 %21, i32 9, i32 12
  %23 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 %22)
  call void @_ZN6String4dtorEv(ptr %1)
  call void @_ZN6String4dtorEv(ptr %2)
  %24 = load i32, ptr %result, align 4
  ret i32 %24
}

define private ptr @_Z14lambda.L2C39.0v() {
  %result = alloca ptr, align 8
  ret ptr @anon.string.0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

define private i1 @_Z14lambda.L7C50.0R6Stringd(ptr %0, double %1) {
  %result = alloca i1, align 1
  %str = alloca ptr, align 8
  %d = alloca double, align 8
  store ptr %0, ptr %str, align 8
  store double %1, ptr %d, align 8
  %3 = load ptr, ptr %str, align 8
  %4 = getelementptr inbounds %struct.String, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = load double, ptr %d, align 8
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %5, double %6)
  %8 = load ptr, ptr %str, align 8
  %9 = call ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(8) %8)
  %10 = call i1 @_Z10isRawEqualPcPc(ptr %9, ptr @anon.string.1)
  br i1 %10, label %land.1.L9C16, label %land.exit.L9C16

land.1.L9C16:                                     ; preds = %2
  %11 = load double, ptr %d, align 8
  %12 = fcmp oeq double %11, 3.140000e+00
  br label %land.exit.L9C16

land.exit.L9C16:                                  ; preds = %land.1.L9C16, %2
  %land_phi = phi i1 [ %10, %2 ], [ %12, %land.1.L9C16 ]
  ret i1 %land_phi
}

declare ptr @_ZN6String6getRawEv(ptr)

declare i1 @_Z10isRawEqualPcPc(ptr, ptr)

declare void @_ZN6String4ctorEPc(ptr, ptr)

define private i16 @_Z15lambda.L13C49.06Strings(%struct.String %0, i16 %1) {
  %result = alloca i16, align 2
  %str = alloca %struct.String, align 8
  %b = alloca i16, align 2
  %3 = alloca i16, align 2
  store %struct.String %0, ptr %str, align 8
  store i16 %1, ptr %b, align 2
  %4 = getelementptr inbounds %struct.String, ptr %str, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = load i16, ptr %b, align 2
  %7 = sext i16 %6 to i32
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, ptr %5, i32 %7)
  %9 = load i16, ptr %b, align 2
  %10 = xor i16 %9, -1
  store i16 %10, ptr %3, align 2
  ret i16 %10
}

declare void @_ZN6String4dtorEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
