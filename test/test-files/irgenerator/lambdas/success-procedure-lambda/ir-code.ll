; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.String = type { ptr, i64, i64 }

@printf.str.0 = private unnamed_addr constant [18 x i8] c"Callback called!\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [35 x i8] c"Callback called with args: %s, %f\0A\00", align 1
@anon.string.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.2 = private unnamed_addr constant [35 x i8] c"Callback called with args: %s, %d\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1

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
  store ptr @_Z14lambda.L2C31.0v, ptr %3, align 8
  %4 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %4, ptr %callbackWithoutArgs, align 8
  %5 = getelementptr inbounds { ptr, ptr }, ptr %callbackWithoutArgs, i32 0, i32 0
  %fct = load ptr, ptr %5, align 8
  call void %fct()
  %6 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr1, i32 0, i32 0
  store ptr @_Z14lambda.L7C44.0RN11__rt_string6StringEd, ptr %6, align 8
  %7 = load { ptr, ptr }, ptr %fat.ptr1, align 8
  store { ptr, ptr } %7, ptr %callbackWithArgs1, align 8
  %8 = getelementptr inbounds { ptr, ptr }, ptr %callbackWithArgs1, i32 0, i32 0
  call void @_ZN6String4ctorEPc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr @anon.string.0)
  %fct2 = load ptr, ptr %8, align 8
  call void %fct2(ptr %1, double 3.140000e+00)
  %9 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr3, i32 0, i32 0
  store ptr @_Z15lambda.L12C41.0N11__rt_string6StringEb, ptr %9, align 8
  %10 = load { ptr, ptr }, ptr %fat.ptr3, align 8
  store { ptr, ptr } %10, ptr %callbackWithArgs2, align 8
  %11 = getelementptr inbounds { ptr, ptr }, ptr %callbackWithArgs2, i32 0, i32 0
  call void @_ZN6String4ctorEPc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr @anon.string.1)
  %12 = load %struct.String, ptr %2, align 8
  %fct4 = load ptr, ptr %11, align 8
  call void %fct4(%struct.String %12, i1 false)
  call void @_ZN6String4dtorEv(ptr %1)
  call void @_ZN6String4dtorEv(ptr %2)
  %13 = load i32, ptr %result, align 4
  ret i32 %13
}

define private void @_Z14lambda.L2C31.0v() {
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

define private void @_Z14lambda.L7C44.0RN11__rt_string6StringEd(ptr %0, double %1) {
  %str = alloca ptr, align 8
  %d = alloca double, align 8
  store ptr %0, ptr %str, align 8
  store double %1, ptr %d, align 8
  %3 = load ptr, ptr %str, align 8
  %4 = getelementptr inbounds %struct.String, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = load double, ptr %d, align 8
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %5, double %6)
  ret void
}

declare void @_ZN6String4ctorEPc(ptr, ptr)

define private void @_Z15lambda.L12C41.0N11__rt_string6StringEb(%struct.String %0, i1 %1) {
  %str = alloca %struct.String, align 8
  %b = alloca i1, align 1
  store %struct.String %0, ptr %str, align 8
  store i1 %1, ptr %b, align 1
  %3 = getelementptr inbounds %struct.String, ptr %str, i32 0, i32 0
  %4 = load ptr, ptr %3, align 8
  %5 = load i1, ptr %b, align 1
  %6 = zext i1 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %4, i32 %6)
  ret void
}

declare void @_ZN6String4dtorEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
