; ModuleID = 'source.spice'
source_filename = "source.spice"

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
  %arg.copy = alloca %struct.String, align 8
  store i32 0, ptr %result, align 4
  store ptr @_Z14lambda.L2C31.0v, ptr %fat.ptr, align 8
  %3 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr poison, ptr %3, align 8
  %4 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %4, ptr %callbackWithoutArgs, align 8
  %fct = load ptr, ptr %callbackWithoutArgs, align 8
  call void %fct()
  store ptr @_Z14lambda.L7C44.0R6Stringd, ptr %fat.ptr1, align 8
  %5 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr1, i32 0, i32 1
  store ptr poison, ptr %5, align 8
  %6 = load { ptr, ptr }, ptr %fat.ptr1, align 8
  store { ptr, ptr } %6, ptr %callbackWithArgs1, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr @anon.string.0)
  %fct2 = load ptr, ptr %callbackWithArgs1, align 8
  call void %fct2(ptr %1, double 3.140000e+00)
  store ptr @_Z15lambda.L12C41.06Stringb, ptr %fat.ptr3, align 8
  %7 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr3, i32 0, i32 1
  store ptr poison, ptr %7, align 8
  %8 = load { ptr, ptr }, ptr %fat.ptr3, align 8
  store { ptr, ptr } %8, ptr %callbackWithArgs2, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr @anon.string.1)
  call void @_ZN6String4ctorERK6String(ptr %arg.copy, ptr %2)
  %9 = load %struct.String, ptr %arg.copy, align 8
  %fct4 = load ptr, ptr %callbackWithArgs2, align 8
  call void %fct4(%struct.String %9, i1 false)
  call void @_ZN6String4dtorEv(ptr %2)
  call void @_ZN6String4dtorEv(ptr %arg.copy)
  call void @_ZN6String4dtorEv(ptr %1)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

define private void @_Z14lambda.L2C31.0v() {
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

define private void @_Z14lambda.L7C44.0R6Stringd(ptr %0, double %1) {
  %str = alloca ptr, align 8
  %d = alloca double, align 8
  store ptr %0, ptr %str, align 8
  store double %1, ptr %d, align 8
  %3 = load ptr, ptr %str, align 8
  %4 = load ptr, ptr %3, align 8
  %5 = load double, ptr %d, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %4, double %5)
  ret void
}

declare void @_ZN6String4ctorEPKc(ptr, ptr)

define private void @_Z15lambda.L12C41.06Stringb(%struct.String %0, i1 %1) {
  %str = alloca %struct.String, align 8
  %b = alloca i1, align 1
  store %struct.String %0, ptr %str, align 8
  store i1 %1, ptr %b, align 1
  %3 = load ptr, ptr %str, align 8
  %4 = load i1, ptr %b, align 1
  %5 = zext i1 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %3, i32 %5)
  ret void
}

declare void @_ZN6String4ctorERK6String(ptr, ptr)

declare void @_ZN6String4dtorEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
