; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Test func 1\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [17 x i8] c"Test func 2: %s\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [12 x i8] c"Result: %d\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [12 x i8] c"param value\00", align 4

define private i32 @_Z8testFuncv() {
  %result = alloca i32, align 4
  %1 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i32 1
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

define private i32 @_Z8testFuncPKc(ptr %0) {
  %result = alloca i32, align 4
  %param = alloca ptr, align 8
  store ptr %0, ptr %param, align 8
  %2 = load ptr, ptr %param, align 8
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr noundef %2)
  ret i32 2
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i32 @_Z8testFuncv()
  store i32 %1, ptr %res, align 4
  %2 = load i32, ptr %res, align 4
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %2)
  %4 = call i32 @_Z8testFuncPKc(ptr @anon.string.0)
  store i32 %4, ptr %res, align 4
  %5 = load i32, ptr %result, align 4
  ret i32 %5
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
