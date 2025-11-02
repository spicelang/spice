; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [7 x i8] c"string\00", align 4
@anon.string.1 = private unnamed_addr constant [7 x i8] c"string\00", align 4
@printf.str.0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4

define private i32 @_Z4testPKc(ptr %0) {
  %result = alloca i32, align 4
  %_input = alloca ptr, align 8
  store ptr %0, ptr %_input, align 8
  ret i32 12
}

define private i32 @_Z6invokePPPFiPKcE(ptr %0) {
  %result = alloca i32, align 4
  %fctPtr = alloca ptr, align 8
  store ptr %0, ptr %fctPtr, align 8
  %2 = load ptr, ptr %fctPtr, align 8
  %3 = load ptr, ptr %2, align 8
  %fct = load ptr, ptr %3, align 8
  %4 = call i32 %fct(ptr @anon.string.0)
  ret i32 %4
}

define private i32 @_Z6invokeRPFiPKcE(ptr %0) {
  %result = alloca i32, align 4
  %fctPtr = alloca ptr, align 8
  store ptr %0, ptr %fctPtr, align 8
  %2 = load ptr, ptr %fctPtr, align 8
  %fct = load ptr, ptr %2, align 8
  %3 = call i32 %fct(ptr @anon.string.1)
  ret i32 %3
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %fat.ptr = alloca { ptr, ptr }, align 8
  %testFct = alloca { ptr, ptr }, align 8
  %testFctPtr = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store ptr @_Z4testPKc, ptr %fat.ptr, align 8
  %1 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr poison, ptr %1, align 8
  %2 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %2, ptr %testFct, align 8
  store ptr %testFct, ptr %testFctPtr, align 8
  %3 = call i32 @_Z6invokePPPFiPKcE(ptr %testFctPtr)
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3)
  %5 = call i32 @_Z6invokeRPFiPKcE(ptr %testFct)
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
