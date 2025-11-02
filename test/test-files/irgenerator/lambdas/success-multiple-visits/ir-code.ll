; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [8 x i8] c"%d, %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [8 x i8] c"%d, %d\0A\00", align 4

define private void @_Z4testv() {
  %t = alloca i32, align 4
  %x = alloca i32, align 4
  %captures = alloca { i32, i32 }, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %l = alloca { ptr, ptr }, align 8
  store i32 123, ptr %t, align 4
  store i32 456, ptr %x, align 4
  %1 = load i32, ptr %t, align 4
  store i32 %1, ptr %captures, align 4
  %2 = load i32, ptr %x, align 4
  %3 = getelementptr inbounds nuw { i32, i32 }, ptr %captures, i32 0, i32 1
  store i32 %2, ptr %3, align 4
  store ptr @_Z14lambda.L3C13.0v, ptr %fat.ptr, align 8
  %4 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %4, align 8
  %5 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %5, ptr %l, align 8
  %6 = getelementptr inbounds nuw { ptr, ptr }, ptr %l, i32 0, i32 1
  %captures1 = load ptr, ptr %6, align 8
  %fct = load ptr, ptr %l, align 8
  call void %fct(ptr %captures1)
  ret void
}

define private void @_Z14lambda.L3C13.0v(ptr noundef nonnull dereferenceable(8) %0) {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  %2 = load ptr, ptr %captures, align 8
  %x = getelementptr inbounds nuw { i32, i32 }, ptr %2, i32 0, i32 1
  %3 = load i32, ptr %x, align 4
  %4 = load i32, ptr %2, align 4
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3, i32 %4)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

define private void @_Z4testi(i32 %0) {
  %t = alloca i32, align 4
  %x = alloca i32, align 4
  %captures = alloca { i32, i32 }, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %l = alloca { ptr, ptr }, align 8
  store i32 %0, ptr %t, align 4
  store i32 456, ptr %x, align 4
  %2 = load i32, ptr %t, align 4
  store i32 %2, ptr %captures, align 4
  %3 = load i32, ptr %x, align 4
  %4 = getelementptr inbounds nuw { i32, i32 }, ptr %captures, i32 0, i32 1
  store i32 %3, ptr %4, align 4
  store ptr @_Z14lambda.L3C13.1v, ptr %fat.ptr, align 8
  %5 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %5, align 8
  %6 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %6, ptr %l, align 8
  %7 = getelementptr inbounds nuw { ptr, ptr }, ptr %l, i32 0, i32 1
  %captures1 = load ptr, ptr %7, align 8
  %fct = load ptr, ptr %l, align 8
  call void %fct(ptr %captures1)
  ret void
}

define private void @_Z14lambda.L3C13.1v(ptr noundef nonnull dereferenceable(8) %0) {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  %2 = load ptr, ptr %captures, align 8
  %x = getelementptr inbounds nuw { i32, i32 }, ptr %2, i32 0, i32 1
  %3 = load i32, ptr %x, align 4
  %4 = load i32, ptr %2, align 4
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %3, i32 %4)
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  call void @_Z4testv()
  call void @_Z4testi(i32 4321)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { nofree nounwind }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
