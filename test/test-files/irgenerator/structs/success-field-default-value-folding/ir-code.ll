; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Test = type { i32, i1, i1, i32, i16, i64, i1, i1, i1, i1, i1, i1, i32, i64, i8, i32, i32, i32, i32 }

@anon.string.0 = private unnamed_addr constant [60 x i8] c"Assertion failed: Condition 't.f1 == 2' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [55 x i8] c"Assertion failed: Condition 't.f2' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition '!t.f3' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [61 x i8] c"Assertion failed: Condition 't.f4 == 11' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 't.f5 == 10s' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [61 x i8] c"Assertion failed: Condition 't.f6 == 2l' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [55 x i8] c"Assertion failed: Condition 't.f7' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [55 x i8] c"Assertion failed: Condition 't.f8' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [55 x i8] c"Assertion failed: Condition 't.f9' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition '!t.f10' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition 't.f11' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition '!t.f12' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f13 == 333' evaluated to false.\00", align 1
@anon.string.13 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f14 == 11l' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 't.f15 == (byte) 63' evaluated to false.\00", align 1
@anon.string.15 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 't.f16 == 13' evaluated to false.\00", align 1
@anon.string.16 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 't.f17 == 13' evaluated to false.\00", align 1
@anon.string.17 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 't.f18 == 14' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 't.f19 == 12' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %t = alloca %struct.Test, align 8
  %1 = alloca i1, align 1
  %2 = alloca i1, align 1
  %3 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  store %struct.Test { i32 2, i1 true, i1 false, i32 11, i16 10, i64 2, i1 true, i1 true, i1 true, i1 false, i1 true, i1 false, i32 333, i64 11, i8 63, i32 13, i32 13, i32 14, i32 12 }, ptr %t, align 8
  %f1_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 0
  %4 = load i32, ptr %f1_addr, align 4
  %5 = icmp eq i32 %4, 2
  br i1 %5, label %assert.exit.L25, label %assert.then.L25, !prof !0

assert.then.L25:                                  ; preds = %0
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L25:                                  ; preds = %0
  %f2_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 1
  %7 = load i1, ptr %f2_addr, align 1
  br i1 %7, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %assert.exit.L25
  %8 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L25
  %f3_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 2
  %9 = load i1, ptr %f3_addr, align 1
  %10 = xor i1 %9, true
  store i1 %10, ptr %1, align 1
  br i1 %10, label %assert.exit.L27, label %assert.then.L27, !prof !0

assert.then.L27:                                  ; preds = %assert.exit.L26
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L27:                                  ; preds = %assert.exit.L26
  %f4_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 3
  %12 = load i32, ptr %f4_addr, align 4
  %13 = icmp eq i32 %12, 11
  br i1 %13, label %assert.exit.L28, label %assert.then.L28, !prof !0

assert.then.L28:                                  ; preds = %assert.exit.L27
  %14 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L27
  %f5_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 4
  %15 = load i16, ptr %f5_addr, align 2
  %16 = icmp eq i16 %15, 10
  br i1 %16, label %assert.exit.L29, label %assert.then.L29, !prof !0

assert.then.L29:                                  ; preds = %assert.exit.L28
  %17 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L29:                                  ; preds = %assert.exit.L28
  %f6_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 5
  %18 = load i64, ptr %f6_addr, align 8
  %19 = icmp eq i64 %18, 2
  br i1 %19, label %assert.exit.L30, label %assert.then.L30, !prof !0

assert.then.L30:                                  ; preds = %assert.exit.L29
  %20 = call i32 (ptr, ...) @printf(ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L29
  %f7_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 6
  %21 = load i1, ptr %f7_addr, align 1
  br i1 %21, label %assert.exit.L31, label %assert.then.L31, !prof !0

assert.then.L31:                                  ; preds = %assert.exit.L30
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L31:                                  ; preds = %assert.exit.L30
  %f8_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 7
  %23 = load i1, ptr %f8_addr, align 1
  br i1 %23, label %assert.exit.L32, label %assert.then.L32, !prof !0

assert.then.L32:                                  ; preds = %assert.exit.L31
  %24 = call i32 (ptr, ...) @printf(ptr @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L31
  %f9_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 8
  %25 = load i1, ptr %f9_addr, align 1
  br i1 %25, label %assert.exit.L33, label %assert.then.L33, !prof !0

assert.then.L33:                                  ; preds = %assert.exit.L32
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L33:                                  ; preds = %assert.exit.L32
  %f10_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 9
  %27 = load i1, ptr %f10_addr, align 1
  %28 = xor i1 %27, true
  store i1 %28, ptr %2, align 1
  br i1 %28, label %assert.exit.L34, label %assert.then.L34, !prof !0

assert.then.L34:                                  ; preds = %assert.exit.L33
  %29 = call i32 (ptr, ...) @printf(ptr @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L33
  %f11_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 10
  %30 = load i1, ptr %f11_addr, align 1
  br i1 %30, label %assert.exit.L35, label %assert.then.L35, !prof !0

assert.then.L35:                                  ; preds = %assert.exit.L34
  %31 = call i32 (ptr, ...) @printf(ptr @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L35:                                  ; preds = %assert.exit.L34
  %f12_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 11
  %32 = load i1, ptr %f12_addr, align 1
  %33 = xor i1 %32, true
  store i1 %33, ptr %3, align 1
  br i1 %33, label %assert.exit.L36, label %assert.then.L36, !prof !0

assert.then.L36:                                  ; preds = %assert.exit.L35
  %34 = call i32 (ptr, ...) @printf(ptr @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L36:                                  ; preds = %assert.exit.L35
  %f13_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 12
  %35 = load i32, ptr %f13_addr, align 4
  %36 = icmp eq i32 %35, 333
  br i1 %36, label %assert.exit.L37, label %assert.then.L37, !prof !0

assert.then.L37:                                  ; preds = %assert.exit.L36
  %37 = call i32 (ptr, ...) @printf(ptr @anon.string.12)
  call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %f14_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 13
  %38 = load i64, ptr %f14_addr, align 8
  %39 = icmp eq i64 %38, 11
  br i1 %39, label %assert.exit.L38, label %assert.then.L38, !prof !0

assert.then.L38:                                  ; preds = %assert.exit.L37
  %40 = call i32 (ptr, ...) @printf(ptr @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L38:                                  ; preds = %assert.exit.L37
  %f15_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 14
  %41 = load i8, ptr %f15_addr, align 1
  %42 = icmp eq i8 %41, 63
  br i1 %42, label %assert.exit.L39, label %assert.then.L39, !prof !0

assert.then.L39:                                  ; preds = %assert.exit.L38
  %43 = call i32 (ptr, ...) @printf(ptr @anon.string.14)
  call void @exit(i32 1)
  unreachable

assert.exit.L39:                                  ; preds = %assert.exit.L38
  %f16_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 15
  %44 = load i32, ptr %f16_addr, align 4
  %45 = icmp eq i32 %44, 13
  br i1 %45, label %assert.exit.L40, label %assert.then.L40, !prof !0

assert.then.L40:                                  ; preds = %assert.exit.L39
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L40:                                  ; preds = %assert.exit.L39
  %f17_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 16
  %47 = load i32, ptr %f17_addr, align 4
  %48 = icmp eq i32 %47, 13
  br i1 %48, label %assert.exit.L41, label %assert.then.L41, !prof !0

assert.then.L41:                                  ; preds = %assert.exit.L40
  %49 = call i32 (ptr, ...) @printf(ptr @anon.string.16)
  call void @exit(i32 1)
  unreachable

assert.exit.L41:                                  ; preds = %assert.exit.L40
  %f18_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 17
  %50 = load i32, ptr %f18_addr, align 4
  %51 = icmp eq i32 %50, 14
  br i1 %51, label %assert.exit.L42, label %assert.then.L42, !prof !0

assert.then.L42:                                  ; preds = %assert.exit.L41
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L42:                                  ; preds = %assert.exit.L41
  %f19_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 18
  %53 = load i32, ptr %f19_addr, align 4
  %54 = icmp eq i32 %53, 12
  br i1 %54, label %assert.exit.L43, label %assert.then.L43, !prof !0

assert.then.L43:                                  ; preds = %assert.exit.L42
  %55 = call i32 (ptr, ...) @printf(ptr @anon.string.18)
  call void @exit(i32 1)
  unreachable

assert.exit.L43:                                  ; preds = %assert.exit.L42
  %56 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %57 = load i32, ptr %result, align 4
  ret i32 %57
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
