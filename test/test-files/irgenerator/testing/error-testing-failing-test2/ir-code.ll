; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'add(1, 2) == 4' evaluated to false.\0A\00", align 4
@allStartMsg0 = private unnamed_addr constant [56 x i8] c"[==========] Running %d test(s) from %d source file(s)\0A\00", align 4
@allEndMsg0 = private unnamed_addr constant [52 x i8] c"[==========] Ran %d test(s) from %d source file(s)\0A\00", align 4
@fileStartMsg0 = private unnamed_addr constant [41 x i8] c"[----------] Running %d test(s) from %s\0A\00", align 4
@fileEndMsg0 = private unnamed_addr constant [38 x i8] c"[----------] Ran %d test(s) from %s\0A\0A\00", align 4
@runMsg0 = private unnamed_addr constant [17 x i8] c"[ RUN      ] %s\0A\00", align 4
@successMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[32m[ PASSED   ]\1B[0m\1B[22m %s\0A\00", align 4
@errorMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[31m[ FAILED   ]\1B[0m\1B[22m %s\0A\00", align 4
@skippedMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[33m[ SKIPPED  ]\1B[0m\1B[22m %s\0A\00", align 4
@fileName0 = private unnamed_addr constant [13 x i8] c"source.spice\00", align 4
@testName0 = private unnamed_addr constant [8 x i8] c"testAdd\00", align 4

define private noundef i32 @_Z3addii(i32 noundef %0, i32 noundef %1) {
  %result = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  store i32 %1, ptr %b, align 4
  %3 = load i32, ptr %b, align 4
  %4 = load i32, ptr %a, align 4
  %5 = add nsw i32 %4, %3
  ret i32 %5
}

define dso_local noundef zeroext i1 @_Z7testAddv() {
  %result = alloca i1, align 1
  %1 = call noundef i32 @_Z3addii(i32 noundef 1, i32 noundef 2)
  %2 = icmp eq i32 %1, 4
  br i1 %2, label %assert.exit.L7, label %assert.then.L7, !prof !5

assert.then.L7:                                   ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L7:                                   ; preds = %0
  ret i1 true
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #1

; Function Attrs: norecurse
define i32 @main() #2 {
  %1 = call i32 (ptr, ...) @printf(ptr @allStartMsg0, i32 1, i32 1)
  %2 = call i32 (ptr, ...) @printf(ptr @fileStartMsg0, i32 1, ptr @fileName0)
  %3 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName0)
  %4 = call i1 @_Z7testAddv()
  %5 = select i1 %4, ptr @successMsg0, ptr @errorMsg0
  %6 = call i32 (ptr, ...) @printf(ptr %5, ptr @testName0)
  %7 = call i32 (ptr, ...) @printf(ptr @fileEndMsg0, i32 1, ptr @fileName0)
  %8 = call i32 (ptr, ...) @printf(ptr @allEndMsg0, i32 1, i32 1)
  %9 = and i1 true, %4
  %10 = xor i1 %9, true
  %11 = zext i1 %10 to i32
  ret i32 %11
}

attributes #0 = { nofree nounwind }
attributes #1 = { cold noreturn nounwind }
attributes #2 = { norecurse }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
