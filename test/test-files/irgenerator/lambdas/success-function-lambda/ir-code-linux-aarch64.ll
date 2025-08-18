; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.String = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [18 x i8] c"Callback called!\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [3 x i8] c"%s\00", align 4
@printf.str.1 = private unnamed_addr constant [35 x i8] c"Callback called with args: %s, %f\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [6 x i8] c"Hello\00", align 4
@printf.str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [6 x i8] c"Hello\00", align 4
@printf.str.3 = private unnamed_addr constant [35 x i8] c"Callback called with args: %s, %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 4

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
  %arg.copy = alloca %struct.String, align 8
  store i32 0, ptr %result, align 4
  store ptr @_Z14lambda.L2C39.0v, ptr %fat.ptr, align 8
  %3 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr poison, ptr %3, align 8
  %4 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %4, ptr %callbackWithoutArgs, align 8
  %fct = load ptr, ptr %callbackWithoutArgs, align 8
  %5 = call ptr %fct()
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %5)
  store ptr @_Z14lambda.L7C50.0R6Stringd, ptr %fat.ptr1, align 8
  %7 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr1, i32 0, i32 1
  store ptr poison, ptr %7, align 8
  %8 = load { ptr, ptr }, ptr %fat.ptr1, align 8
  store { ptr, ptr } %8, ptr %callbackWithArgs1, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr @anon.string.2)
  %fct2 = load ptr, ptr %callbackWithArgs1, align 8
  %9 = call i1 %fct2(ptr %1, double 3.140000e+00)
  %10 = zext i1 %9 to i32
  %11 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %10)
  store ptr @_Z15lambda.L13C49.06Strings, ptr %fat.ptr3, align 8
  %12 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr3, i32 0, i32 1
  store ptr poison, ptr %12, align 8
  %13 = load { ptr, ptr }, ptr %fat.ptr3, align 8
  store { ptr, ptr } %13, ptr %callbackWithArgs2, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr @anon.string.3)
  call void @_ZN6String4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(24) %arg.copy, ptr %2)
  %14 = load %struct.String, ptr %arg.copy, align 8
  %fct4 = load ptr, ptr %callbackWithArgs2, align 8
  %15 = call i16 %fct4(%struct.String %14, i16 321)
  %16 = xor i16 %15, 956
  %17 = sext i16 %16 to i32
  %18 = icmp eq i32 %17, 1
  %19 = select i1 %18, i32 9, i32 12
  %20 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 %19)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %2)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %arg.copy)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %1)
  %21 = load i32, ptr %result, align 4
  ret i32 %21
}

define private ptr @_Z14lambda.L2C39.0v() {
  %result = alloca ptr, align 8
  ret ptr @anon.string.0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

define private i1 @_Z14lambda.L7C50.0R6Stringd(ptr %0, double %1) {
  %result = alloca i1, align 1
  %str = alloca ptr, align 8
  %d = alloca double, align 8
  store ptr %0, ptr %str, align 8
  store double %1, ptr %d, align 8
  %3 = load ptr, ptr %str, align 8
  %4 = load ptr, ptr %3, align 8
  %5 = load double, ptr %d, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %4, double %5)
  %7 = load ptr, ptr %str, align 8
  %8 = call ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %7)
  %9 = call i1 @_Z10isRawEqualPKcPKc(ptr %8, ptr @anon.string.1)
  br i1 %9, label %land.1.L9C16, label %land.exit.L9C16

land.1.L9C16:                                     ; preds = %2
  %10 = load double, ptr %d, align 8
  %11 = fcmp oeq double %10, 3.140000e+00
  br label %land.exit.L9C16

land.exit.L9C16:                                  ; preds = %land.1.L9C16, %2
  %land_phi = phi i1 [ %9, %2 ], [ %11, %land.1.L9C16 ]
  ret i1 %land_phi
}

declare ptr @_ZN6String6getRawEv(ptr)

declare i1 @_Z10isRawEqualPKcPKc(ptr, ptr)

declare void @_ZN6String4ctorEPKc(ptr, ptr)

define private i16 @_Z15lambda.L13C49.06Strings(%struct.String %0, i16 %1) {
  %result = alloca i16, align 4
  %str = alloca %struct.String, align 8
  %b = alloca i16, align 4
  store %struct.String %0, ptr %str, align 8
  store i16 %1, ptr %b, align 2
  %3 = load ptr, ptr %str, align 8
  %4 = load i16, ptr %b, align 2
  %5 = sext i16 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, ptr %3, i32 %5)
  %7 = load i16, ptr %b, align 2
  %8 = xor i16 %7, -1
  ret i16 %8
}

declare void @_ZN6String4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(24), ptr)

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
