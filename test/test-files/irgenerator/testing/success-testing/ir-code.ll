; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 'add(1, 2) == 3' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 'add(2, 2) == 4' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 'add(3, 2) == 5' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'add(5, -4) == 1' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'add(2, 8) == 10' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'add(-3, 5) == 2' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'sub(1, 2) == -1' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 'sub(2, 2) == 0' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 'sub(3, 2) == 1' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'sub(5, -4) == 9' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'sub(2, 8) == -6' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'sub(-3, 5) == -8' evaluated to false.\00", align 1
@allStartMsg0 = private unnamed_addr constant [56 x i8] c"[==========] Running %d test(s) from %d source file(s)\0A\00", align 1
@allEndMsg0 = private unnamed_addr constant [52 x i8] c"[==========] Ran %d test(s) from %d source file(s)\0A\00", align 1
@fileStartMsg0 = private unnamed_addr constant [41 x i8] c"[----------] Running %d test(s) from %s\0A\00", align 1
@fileEndMsg0 = private unnamed_addr constant [38 x i8] c"[----------] Ran %d test(s) from %s\0A\0A\00", align 1
@runMsg0 = private unnamed_addr constant [17 x i8] c"[ RUN      ] %s\0A\00", align 1
@successMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[32m[ PASSED   ]\1B[0m\1B[22m %s\0A\00", align 1
@errorMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[31m[ FAILED   ]\1B[0m\1B[22m %s\0A\00", align 1
@skippedMsg0 = private unnamed_addr constant [35 x i8] c"\1B[1m\1B[33m[ SKIPPED  ]\1B[0m\1B[22m %s\0A\00", align 1
@fileName0 = private unnamed_addr constant [13 x i8] c"source.spice\00", align 1
@testName0 = private unnamed_addr constant [53 x i8] c"testAdd1 (Simple 'happy-path' test for add function)\00", align 1
@testName1 = private unnamed_addr constant [9 x i8] c"testAdd2\00", align 1
@testName2 = private unnamed_addr constant [9 x i8] c"testSub1\00", align 1
@testName3 = private unnamed_addr constant [9 x i8] c"testSub2\00", align 1

define private i32 @_Z3addii(i32 %0, i32 %1) {
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

define private i32 @_Z3subii(i32 %0, i32 %1) {
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

define dso_local i1 @_Z8testAdd1v() {
  %result = alloca i1, align 1
  %1 = call i32 @_Z3addii(i32 1, i32 2)
  %2 = icmp eq i32 %1, 3
  br i1 %2, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %0
  %4 = call i32 @_Z3addii(i32 2, i32 2)
  %5 = icmp eq i32 %4, 4
  br i1 %5, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L12
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L12
  %7 = call i32 @_Z3addii(i32 3, i32 2)
  %8 = icmp eq i32 %7, 5
  br i1 %8, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L13
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
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
  br i1 %2, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %0
  %4 = call i32 @_Z3addii(i32 2, i32 8)
  %5 = icmp eq i32 %4, 10
  br i1 %5, label %assert.exit.L21, label %assert.then.L21, !prof !0

assert.then.L21:                                  ; preds = %assert.exit.L20
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L20
  %7 = call i32 @_Z3addii(i32 -3, i32 5)
  %8 = icmp eq i32 %7, 2
  br i1 %8, label %assert.exit.L22, label %assert.then.L22, !prof !0

assert.then.L22:                                  ; preds = %assert.exit.L21
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L21
  ret i1 false
}

define dso_local i1 @_Z8testSub1v() {
  %result = alloca i1, align 1
  %1 = call i32 @_Z3subii(i32 1, i32 2)
  %2 = icmp eq i32 %1, -1
  br i1 %2, label %assert.exit.L28, label %assert.then.L28, !prof !0

assert.then.L28:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L28:                                  ; preds = %0
  %4 = call i32 @_Z3subii(i32 2, i32 2)
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %assert.exit.L29, label %assert.then.L29, !prof !0

assert.then.L29:                                  ; preds = %assert.exit.L28
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L29:                                  ; preds = %assert.exit.L28
  %7 = call i32 @_Z3subii(i32 3, i32 2)
  %8 = icmp eq i32 %7, 1
  br i1 %8, label %assert.exit.L30, label %assert.then.L30, !prof !0

assert.then.L30:                                  ; preds = %assert.exit.L29
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L29
  ret i1 false
}

define dso_local i1 @_Z8testSub2v() {
  %result = alloca i1, align 1
  %1 = call i32 @_Z3subii(i32 5, i32 -4)
  %2 = icmp eq i32 %1, 9
  br i1 %2, label %assert.exit.L36, label %assert.then.L36, !prof !0

assert.then.L36:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L36:                                  ; preds = %0
  %4 = call i32 @_Z3subii(i32 2, i32 8)
  %5 = icmp eq i32 %4, -6
  br i1 %5, label %assert.exit.L37, label %assert.then.L37, !prof !0

assert.then.L37:                                  ; preds = %assert.exit.L36
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %7 = call i32 @_Z3subii(i32 -3, i32 5)
  %8 = icmp eq i32 %7, -8
  br i1 %8, label %assert.exit.L38, label %assert.then.L38, !prof !0

assert.then.L38:                                  ; preds = %assert.exit.L37
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L38:                                  ; preds = %assert.exit.L37
  ret i1 true
}

; Function Attrs: norecurse
define i32 @main() #2 {
  %result = alloca i32, align 4
  store i1 true, ptr %result, align 1
  %1 = call i32 (ptr, ...) @printf(ptr @allStartMsg0, i32 4, i32 1)
  %2 = call i32 (ptr, ...) @printf(ptr @fileStartMsg0, i32 4, ptr @fileName0)
  %3 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName0)
  %4 = call i1 @_Z8testAdd1v()
  %5 = load i32, ptr %result, align 4
  %6 = zext i1 %4 to i32
  %7 = and i32 %5, %6
  store i32 %7, ptr %result, align 4
  %8 = select i1 %4, ptr @successMsg0, ptr @errorMsg0
  %9 = call i32 (ptr, ...) @printf(ptr %8, ptr @testName0)
  %10 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName1)
  %11 = call i32 (ptr, ...) @printf(ptr @skippedMsg0, ptr @testName1)
  %12 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName2)
  %13 = call i1 @_Z8testSub1v()
  %14 = load i32, ptr %result, align 4
  %15 = zext i1 %13 to i32
  %16 = and i32 %14, %15
  store i32 %16, ptr %result, align 4
  %17 = select i1 %13, ptr @successMsg0, ptr @errorMsg0
  %18 = call i32 (ptr, ...) @printf(ptr %17, ptr @testName2)
  %19 = call i32 (ptr, ...) @printf(ptr @runMsg0, ptr @testName3)
  %20 = call i1 @_Z8testSub2v()
  %21 = load i32, ptr %result, align 4
  %22 = zext i1 %20 to i32
  %23 = and i32 %21, %22
  store i32 %23, ptr %result, align 4
  %24 = select i1 %20, ptr @successMsg0, ptr @errorMsg0
  %25 = call i32 (ptr, ...) @printf(ptr %24, ptr @testName3)
  %26 = call i32 (ptr, ...) @printf(ptr @fileEndMsg0, i32 4, ptr @fileName0)
  %27 = call i32 (ptr, ...) @printf(ptr @allEndMsg0, i32 4, i32 1)
  %28 = load i32, ptr %result, align 4
  ret i32 %28
}

attributes #0 = { nofree nounwind }
attributes #1 = { cold noreturn nounwind }
attributes #2 = { norecurse }

!0 = !{!"branch_weights", i32 2000, i32 1}
