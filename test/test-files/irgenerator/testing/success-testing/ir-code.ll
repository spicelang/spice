; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'add(1, 2) == 3' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'add(2, 2) == 4' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'add(3, 2) == 5' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'add(5, -4) == 1' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'add(2, 8) == 10' evaluated to false.\0A\00", align 4
@anon.string.5 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'add(-3, 5) == 2' evaluated to false.\0A\00", align 4
@anon.string.6 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'sub(1, 2) == -1' evaluated to false.\0A\00", align 4
@anon.string.7 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'sub(2, 2) == 0' evaluated to false.\0A\00", align 4
@anon.string.8 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'sub(3, 2) == 1' evaluated to false.\0A\00", align 4
@anon.string.9 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'sub(5, -4) == 9' evaluated to false.\0A\00", align 4
@anon.string.10 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'sub(2, 8) == -6' evaluated to false.\0A\00", align 4
@anon.string.11 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'sub(-3, 5) == -8' evaluated to false.\0A\00", align 4
@allStartMsg0 = private unnamed_addr constant [56 x i8] c"[==========] Running %d test(s) from %d source file(s)\0A\00", align 4
@allEndMsg0 = private unnamed_addr constant [52 x i8] c"[==========] Ran %d test(s) from %d source file(s)\0A\00", align 4
@fileStartMsg0 = private unnamed_addr constant [41 x i8] c"[----------] Running %d test(s) from %s\0A\00", align 4
@fileEndMsg0 = private unnamed_addr constant [38 x i8] c"[----------] Ran %d test(s) from %s\0A\0A\00", align 4
@runMsg0 = private unnamed_addr constant [17 x i8] c"[ RUN      ] %s\0A\00", align 4
@successMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[32m[ PASSED   ]\1B[0m\1B[22m %s\0A\00", align 4
@errorMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[31m[ FAILED   ]\1B[0m\1B[22m %s\0A\00", align 4
@skippedMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[33m[ SKIPPED  ]\1B[0m\1B[22m %s\0A\00", align 4
@fileName0 = private unnamed_addr constant [13 x i8] c"source.spice\00", align 4
@testName0 = private unnamed_addr constant [53 x i8] c"testAdd1 (Simple 'happy-path' test for add function)\00", align 4
@testName1 = private unnamed_addr constant [9 x i8] c"testAdd2\00", align 4
@testName2 = private unnamed_addr constant [9 x i8] c"testSub1\00", align 4
@testName3 = private unnamed_addr constant [9 x i8] c"testSub2\00", align 4

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

define private noundef i32 @_Z3subii(i32 noundef %0, i32 noundef %1) {
  %result = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  store i32 %1, ptr %b, align 4
  %3 = load i32, ptr %b, align 4
  %4 = load i32, ptr %a, align 4
  %5 = sub nsw i32 %4, %3
  ret i32 %5
}

define dso_local noundef zeroext i1 @_Z8testAdd1v() {
  %result = alloca i1, align 1
  %1 = call noundef i32 @_Z3addii(i32 noundef 1, i32 noundef 2)
  %2 = icmp eq i32 %1, 3
  br i1 %2, label %assert.exit.L12, label %assert.then.L12, !prof !5

assert.then.L12:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %0
  %4 = call noundef i32 @_Z3addii(i32 noundef 2, i32 noundef 2)
  %5 = icmp eq i32 %4, 4
  br i1 %5, label %assert.exit.L13, label %assert.then.L13, !prof !5

assert.then.L13:                                  ; preds = %assert.exit.L12
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L12
  %7 = call noundef i32 @_Z3addii(i32 noundef 3, i32 noundef 2)
  %8 = icmp eq i32 %7, 5
  br i1 %8, label %assert.exit.L14, label %assert.then.L14, !prof !5

assert.then.L14:                                  ; preds = %assert.exit.L13
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  ret i1 true
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #1

define dso_local noundef zeroext i1 @_Z8testAdd2v() {
  %result = alloca i1, align 1
  %1 = call noundef i32 @_Z3addii(i32 noundef 5, i32 noundef -4)
  %2 = icmp eq i32 %1, 1
  br i1 %2, label %assert.exit.L20, label %assert.then.L20, !prof !5

assert.then.L20:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %0
  %4 = call noundef i32 @_Z3addii(i32 noundef 2, i32 noundef 8)
  %5 = icmp eq i32 %4, 10
  br i1 %5, label %assert.exit.L21, label %assert.then.L21, !prof !5

assert.then.L21:                                  ; preds = %assert.exit.L20
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L20
  %7 = call noundef i32 @_Z3addii(i32 noundef -3, i32 noundef 5)
  %8 = icmp eq i32 %7, 2
  br i1 %8, label %assert.exit.L22, label %assert.then.L22, !prof !5

assert.then.L22:                                  ; preds = %assert.exit.L21
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L21
  ret i1 false
}

define dso_local noundef zeroext i1 @_Z8testSub1v() {
  %result = alloca i1, align 1
  %1 = call noundef i32 @_Z3subii(i32 noundef 1, i32 noundef 2)
  %2 = icmp eq i32 %1, -1
  br i1 %2, label %assert.exit.L28, label %assert.then.L28, !prof !5

assert.then.L28:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L28:                                  ; preds = %0
  %4 = call noundef i32 @_Z3subii(i32 noundef 2, i32 noundef 2)
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %assert.exit.L29, label %assert.then.L29, !prof !5

assert.then.L29:                                  ; preds = %assert.exit.L28
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L29:                                  ; preds = %assert.exit.L28
  %7 = call noundef i32 @_Z3subii(i32 noundef 3, i32 noundef 2)
  %8 = icmp eq i32 %7, 1
  br i1 %8, label %assert.exit.L30, label %assert.then.L30, !prof !5

assert.then.L30:                                  ; preds = %assert.exit.L29
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L29
  ret i1 true
}

define dso_local noundef zeroext i1 @_Z8testSub2v() {
  %result = alloca i1, align 1
  %1 = call noundef i32 @_Z3subii(i32 noundef 5, i32 noundef -4)
  %2 = icmp eq i32 %1, 9
  br i1 %2, label %assert.exit.L36, label %assert.then.L36, !prof !5

assert.then.L36:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L36:                                  ; preds = %0
  %4 = call noundef i32 @_Z3subii(i32 noundef 2, i32 noundef 8)
  %5 = icmp eq i32 %4, -6
  br i1 %5, label %assert.exit.L37, label %assert.then.L37, !prof !5

assert.then.L37:                                  ; preds = %assert.exit.L36
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %7 = call noundef i32 @_Z3subii(i32 noundef -3, i32 noundef 5)
  %8 = icmp eq i32 %7, -8
  br i1 %8, label %assert.exit.L38, label %assert.then.L38, !prof !5

assert.then.L38:                                  ; preds = %assert.exit.L37
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L38:                                  ; preds = %assert.exit.L37
  ret i1 true
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define i32 @main() #2 {
  %1 = call i32 (ptr, ...) @printf(ptr @allStartMsg0, i32 4, i32 1)
  %2 = call i32 (ptr, ...) @printf(ptr @fileStartMsg0, i32 4, ptr @fileName0)
  %3 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName0)
  %4 = call i1 @_Z8testAdd1v()
  %5 = select i1 %4, ptr @successMsg0, ptr @errorMsg0
  %6 = call i32 (ptr, ...) @printf(ptr %5, ptr @testName0)
  %7 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName1)
  %8 = call i32 (ptr, ...) @printf(ptr @skippedMsg0, ptr @testName1)
  %9 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName2)
  %10 = call i1 @_Z8testSub1v()
  %11 = select i1 %10, ptr @successMsg0, ptr @errorMsg0
  %12 = call i32 (ptr, ...) @printf(ptr %11, ptr @testName2)
  %13 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName3)
  %14 = call i1 @_Z8testSub2v()
  %15 = select i1 %14, ptr @successMsg0, ptr @errorMsg0
  %16 = call i32 (ptr, ...) @printf(ptr %15, ptr @testName3)
  %17 = call i32 (ptr, ...) @printf(ptr @fileEndMsg0, i32 4, ptr @fileName0)
  %18 = call i32 (ptr, ...) @printf(ptr @allEndMsg0, i32 4, i32 1)
  %19 = and i1 true, %4
  %20 = and i1 %19, %10
  %21 = and i1 %20, %14
  %22 = xor i1 %21, true
  %23 = zext i1 %22 to i32
  ret i32 %23
}

attributes #0 = { nofree nounwind }
attributes #1 = { cold noreturn nounwind }
attributes #2 = { mustprogress noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
