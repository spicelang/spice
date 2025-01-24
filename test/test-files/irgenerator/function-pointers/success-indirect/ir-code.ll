; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [7 x i8] c"string\00", align 1
@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define private i32 @_Z4testPKc(ptr %0) {
  %result = alloca i32, align 4
  %_input = alloca ptr, align 8
  store ptr %0, ptr %_input, align 8
  ret i32 12
}

define private i32 @_Z6invokePFiPKcE({ ptr, ptr } %0) {
  %result = alloca i32, align 4
  %fctPtr = alloca { ptr, ptr }, align 8
  store { ptr, ptr } %0, ptr %fctPtr, align 8
  %fct = load ptr, ptr %fctPtr, align 8
  %2 = call i32 %fct(ptr @anon.string.0)
  ret i32 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %fat.ptr = alloca { ptr, ptr }, align 8
  %testFct = alloca { ptr, ptr }, align 8
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store ptr @_Z4testPKc, ptr %fat.ptr, align 8
  %1 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr poison, ptr %1, align 8
  %2 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %2, ptr %testFct, align 8
  %3 = load { ptr, ptr }, ptr %testFct, align 8
  %4 = call i32 @_Z6invokePFiPKcE({ ptr, ptr } %3)
  store i32 %4, ptr %i, align 4
  %5 = load i32, ptr %i, align 4
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
