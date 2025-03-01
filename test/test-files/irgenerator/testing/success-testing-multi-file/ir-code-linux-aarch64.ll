; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'add(1, 2) == 3' evaluated to false.\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'add(2, 2) == 4' evaluated to false.\0A\00", align 1
@anon.string.2 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'add(3, 2) == 5' evaluated to false.\0A\00", align 1
@anon.string.3 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'add(5, -4) == 1' evaluated to false.\0A\00", align 1
@anon.string.4 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'add(2, 8) == 10' evaluated to false.\0A\00", align 1
@anon.string.5 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'add(-3, 5) == 2' evaluated to false.\0A\00", align 1
@allStartMsg0 = private unnamed_addr constant [56 x i8] c"[==========] Running %d test(s) from %d source file(s)\0A\00", align 1
@allEndMsg0 = private unnamed_addr constant [52 x i8] c"[==========] Ran %d test(s) from %d source file(s)\0A\00", align 1
@fileStartMsg0 = private unnamed_addr constant [41 x i8] c"[----------] Running %d test(s) from %s\0A\00", align 1
@fileEndMsg0 = private unnamed_addr constant [38 x i8] c"[----------] Ran %d test(s) from %s\0A\0A\00", align 1
@runMsg0 = private unnamed_addr constant [17 x i8] c"[ RUN      ] %s\0A\00", align 1
@successMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[32m[ PASSED   ]\1B[0m\1B[22m %s\0A\00", align 1
@errorMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[31m[ FAILED   ]\1B[0m\1B[22m %s\0A\00", align 1
@skippedMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[33m[ SKIPPED  ]\1B[0m\1B[22m %s\0A\00", align 1
@fileName0 = private unnamed_addr constant [14 x i8] c"source2.spice\00", align 1
@testName0 = private unnamed_addr constant [9 x i8] c"testSub1\00", align 1
@testName1 = private unnamed_addr constant [9 x i8] c"testSub2\00", align 1
@fileName1 = private unnamed_addr constant [13 x i8] c"source.spice\00", align 1
@testName2 = private unnamed_addr constant [9 x i8] c"testAdd1\00", align 1
@testName3 = private unnamed_addr constant [9 x i8] c"testAdd2\00", align 1

define private i32 @_Z3addii(i32 %0, i32 %1) {
  %result = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  store i32 %1, ptr %b, align 4
  %3 = load i32, ptr %a, align 4
  %4 = load i32, ptr %b, align 4
  %5 = add nsw i32 %3, %4
  ret i32 %5
}

define dso_local i1 @_Z8testAdd1v() {
  %result = alloca i1, align 1
  %1 = call i32 @_Z3addii(i32 1, i32 2)
  %2 = icmp eq i32 %1, 3
  br i1 %2, label %assert.exit.L9, label %assert.then.L9, !prof !5

assert.then.L9:                                   ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %0
  %4 = call i32 @_Z3addii(i32 2, i32 2)
  %5 = icmp eq i32 %4, 4
  br i1 %5, label %assert.exit.L10, label %assert.then.L10, !prof !5

assert.then.L10:                                  ; preds = %assert.exit.L9
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L9
  %7 = call i32 @_Z3addii(i32 3, i32 2)
  %8 = icmp eq i32 %7, 5
  br i1 %8, label %assert.exit.L11, label %assert.then.L11, !prof !5

assert.then.L11:                                  ; preds = %assert.exit.L10
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L11:                                  ; preds = %assert.exit.L10
  ret i1 true
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #1

define dso_local i1 @_Z8testAdd2v() {
  %result = alloca i1, align 1
  %1 = call i32 @_Z3addii(i32 5, i32 -4)
  %2 = icmp eq i32 %1, 1
  br i1 %2, label %assert.exit.L17, label %assert.then.L17, !prof !5

assert.then.L17:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %0
  %4 = call i32 @_Z3addii(i32 2, i32 8)
  %5 = icmp eq i32 %4, 10
  br i1 %5, label %assert.exit.L18, label %assert.then.L18, !prof !5

assert.then.L18:                                  ; preds = %assert.exit.L17
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L17
  %7 = call i32 @_Z3addii(i32 -3, i32 5)
  %8 = icmp eq i32 %7, 2
  br i1 %8, label %assert.exit.L19, label %assert.then.L19, !prof !5

assert.then.L19:                                  ; preds = %assert.exit.L18
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L18
  ret i1 false
}

; Function Attrs: norecurse
define i32 @main() #2 {
  %result = alloca i32, align 4
  store i1 true, ptr %result, align 1
  %1 = call i32 (ptr, ...) @printf(ptr @allStartMsg0, i32 4, i32 2)
  %2 = call i32 (ptr, ...) @printf(ptr @fileStartMsg0, i32 2, ptr @fileName0)
  %3 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName0)
  %4 = call i1 @_Z8testSub1v()
  %5 = load i32, ptr %result, align 4
  %6 = zext i1 %4 to i32
  %7 = and i32 %5, %6
  store i32 %7, ptr %result, align 4
  %8 = select i1 %4, ptr @successMsg0, ptr @errorMsg0
  %9 = call i32 (ptr, ...) @printf(ptr %8, ptr @testName0)
  %10 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName1)
  %11 = call i1 @_Z8testSub2v()
  %12 = load i32, ptr %result, align 4
  %13 = zext i1 %11 to i32
  %14 = and i32 %12, %13
  store i32 %14, ptr %result, align 4
  %15 = select i1 %11, ptr @successMsg0, ptr @errorMsg0
  %16 = call i32 (ptr, ...) @printf(ptr %15, ptr @testName1)
  %17 = call i32 (ptr, ...) @printf(ptr @fileEndMsg0, i32 2, ptr @fileName0)
  %18 = call i32 (ptr, ...) @printf(ptr @fileStartMsg0, i32 2, ptr @fileName1)
  %19 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName2)
  %20 = call i1 @_Z8testAdd1v()
  %21 = load i32, ptr %result, align 4
  %22 = zext i1 %20 to i32
  %23 = and i32 %21, %22
  store i32 %23, ptr %result, align 4
  %24 = select i1 %20, ptr @successMsg0, ptr @errorMsg0
  %25 = call i32 (ptr, ...) @printf(ptr %24, ptr @testName2)
  %26 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName3)
  %27 = call i1 @_Z8testAdd2v()
  %28 = load i32, ptr %result, align 4
  %29 = zext i1 %27 to i32
  %30 = and i32 %28, %29
  store i32 %30, ptr %result, align 4
  %31 = select i1 %27, ptr @successMsg0, ptr @errorMsg0
  %32 = call i32 (ptr, ...) @printf(ptr %31, ptr @testName3)
  %33 = call i32 (ptr, ...) @printf(ptr @fileEndMsg0, i32 2, ptr @fileName1)
  %34 = call i32 (ptr, ...) @printf(ptr @allEndMsg0, i32 4, i32 2)
  %35 = load i32, ptr %result, align 4
  ret i32 %35
}

declare i1 @_Z8testSub1v()

declare i1 @_Z8testSub2v()

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
!5 = !{!"branch_weights", i32 2000, i32 1}
