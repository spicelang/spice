; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.String = type { ptr, i64, i64 }

@printf.str.0 = private unnamed_addr constant [18 x i8] c"Callback called!\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [35 x i8] c"Callback called with args: %s, %f\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 4
@printf.str.2 = private unnamed_addr constant [35 x i8] c"Callback called with args: %s, %d\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %callbackWithoutArgs = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr1 = alloca { ptr, ptr, i64 }, align 8
  %callbackWithArgs1 = alloca { ptr, ptr, i64 }, align 8
  %1 = alloca %struct.String, align 8
  %fat.ptr4 = alloca { ptr, ptr, i64 }, align 8
  %callbackWithArgs2 = alloca { ptr, ptr, i64 }, align 8
  %2 = alloca %struct.String, align 8
  %arg.copy = alloca %struct.String, align 8
  store i32 0, ptr %result, align 4
  store ptr @_Z14lambda.L2C31.0v, ptr %fat.ptr, align 8
  %3 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1
  store ptr null, ptr %3, align 8
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2
  store i64 0, ptr %4, align 8
  %5 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8
  store { ptr, ptr, i64 } %5, ptr %callbackWithoutArgs, align 8
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %callbackWithoutArgs, i32 0, i32 1
  %captures = load ptr, ptr %6, align 8
  %fct = load ptr, ptr %callbackWithoutArgs, align 8
  call void %fct(ptr %captures)
  store ptr @_Z14lambda.L7C44.0R6Stringd, ptr %fat.ptr1, align 8
  %7 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr1, i32 0, i32 1
  store ptr null, ptr %7, align 8
  %8 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr1, i32 0, i32 2
  store i64 0, ptr %8, align 8
  %9 = load { ptr, ptr, i64 }, ptr %fat.ptr1, align 8
  store { ptr, ptr, i64 } %9, ptr %callbackWithArgs1, align 8
  %10 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %callbackWithArgs1, i32 0, i32 1
  %captures2 = load ptr, ptr %10, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr noundef @anon.string.0)
  %fct3 = load ptr, ptr %callbackWithArgs1, align 8
  call void %fct3(ptr %captures2, ptr %1, double 3.140000e+00)
  store ptr @_Z15lambda.L12C41.06Stringb, ptr %fat.ptr4, align 8
  %11 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr4, i32 0, i32 1
  store ptr null, ptr %11, align 8
  %12 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr4, i32 0, i32 2
  store i64 0, ptr %12, align 8
  %13 = load { ptr, ptr, i64 }, ptr %fat.ptr4, align 8
  store { ptr, ptr, i64 } %13, ptr %callbackWithArgs2, align 8
  %14 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %callbackWithArgs2, i32 0, i32 1
  %captures5 = load ptr, ptr %14, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr noundef @anon.string.1)
  call void @_ZN6String4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(24) %arg.copy, ptr %2)
  %15 = load %struct.String, ptr %arg.copy, align 8
  %fct6 = load ptr, ptr %callbackWithArgs2, align 8
  call void %fct6(ptr %captures5, %struct.String %15, i1 false)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %2)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %arg.copy)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %1)
  %16 = load i32, ptr %result, align 4
  ret i32 %16
}

define private void @_Z14lambda.L2C31.0v(ptr %0) {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

define private void @_Z14lambda.L7C44.0R6Stringd(ptr %0, ptr %1, double %2) {
  %captures = alloca ptr, align 8
  %str = alloca ptr, align 8
  %d = alloca double, align 8
  store ptr %0, ptr %captures, align 8
  store ptr %1, ptr %str, align 8
  store double %2, ptr %d, align 8
  %4 = load ptr, ptr %str, align 8
  %5 = load ptr, ptr %4, align 8
  %6 = load double, ptr %d, align 8
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr noundef %5, double noundef %6)
  ret void
}

declare void @_ZN6String4ctorEPKc(ptr, ptr)

define private void @_Z15lambda.L12C41.06Stringb(ptr %0, %struct.String %1, i1 %2) {
  %captures = alloca ptr, align 8
  %str = alloca %struct.String, align 8
  %b = alloca i1, align 1
  store ptr %0, ptr %captures, align 8
  store %struct.String %1, ptr %str, align 8
  store i1 %2, ptr %b, align 1
  %4 = load ptr, ptr %str, align 8
  %5 = load i1, ptr %b, align 1
  %6 = zext i1 %5 to i32
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr noundef %4, i32 noundef %6)
  ret void
}

declare void @_ZN6String4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(24), ptr)

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
