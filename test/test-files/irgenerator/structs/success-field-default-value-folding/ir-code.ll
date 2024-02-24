; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32, i1, i1, i32, i16, i64, i1, i1, i1, i1, i1, i1, i32, i64, i8, i32, i32, i32, i32, i32 }

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
@anon.string.19 = private unnamed_addr constant [82 x i8] c"Assertion failed: Condition 't.f20 == SpecifierType::COMPOSE' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 1

; Function Attrs: norecurse
define void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(72) %0) #0 {
  %this = alloca ptr, align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  %6 = load ptr, ptr %this, align 8
  %7 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 0
  store i32 2, ptr %7, align 4
  %8 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 1
  br i1 false, label %lor.exit.L14C15, label %lor.1.L14C15

lor.1.L14C15:                                     ; preds = %1
  br i1 false, label %lor.exit.L14C15, label %lor.2.L14C15

lor.2.L14C15:                                     ; preds = %lor.1.L14C15
  br label %lor.exit.L14C15

lor.exit.L14C15:                                  ; preds = %lor.2.L14C15, %lor.1.L14C15, %1
  %lor_phi = phi i1 [ false, %1 ], [ false, %lor.1.L14C15 ], [ true, %lor.2.L14C15 ]
  store i1 %lor_phi, ptr %8, align 1
  %9 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 2
  br i1 true, label %land.1.L15C15, label %land.exit.L15C15

land.1.L15C15:                                    ; preds = %lor.exit.L14C15
  br i1 false, label %land.2.L15C15, label %land.exit.L15C15

land.2.L15C15:                                    ; preds = %land.1.L15C15
  br label %land.exit.L15C15

land.exit.L15C15:                                 ; preds = %land.2.L15C15, %land.1.L15C15, %lor.exit.L14C15
  %land_phi = phi i1 [ true, %lor.exit.L14C15 ], [ false, %land.1.L15C15 ], [ true, %land.2.L15C15 ]
  store i1 %land_phi, ptr %9, align 1
  %10 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 3
  store i32 11, ptr %10, align 4
  %11 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 4
  store i16 10, ptr %11, align 2
  %12 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 5
  store i64 2, ptr %12, align 8
  %13 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 6
  store i1 true, ptr %13, align 1
  %14 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 7
  store i1 true, ptr %14, align 1
  %15 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 8
  store i1 true, ptr %15, align 1
  %16 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 9
  store i1 false, ptr %16, align 1
  %17 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 10
  store i1 true, ptr %17, align 1
  %18 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 11
  store i1 false, ptr %18, align 1
  %19 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 12
  store i32 333, ptr %19, align 4
  %20 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 13
  store i64 11, ptr %20, align 8
  %21 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 14
  store i8 63, ptr %21, align 1
  %22 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 15
  store i32 14, ptr %2, align 4
  store i32 13, ptr %22, align 4
  %23 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 16
  store i32 12, ptr %3, align 4
  store i32 13, ptr %23, align 4
  %24 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 17
  store i32 14, ptr %4, align 4
  store i32 14, ptr %24, align 4
  %25 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 18
  store i32 12, ptr %5, align 4
  store i32 12, ptr %25, align 4
  %26 = getelementptr inbounds %struct.Test, ptr %6, i32 0, i32 19
  store i32 7, ptr %26, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %t = alloca %struct.Test, align 8
  %1 = alloca i1, align 1
  %2 = alloca i1, align 1
  %3 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  call void @_ZN4Test4ctorEv(ptr %t)
  %f1_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 0
  %4 = load i32, ptr %f1_addr, align 4
  %5 = icmp eq i32 %4, 2
  br i1 %5, label %assert.exit.L37, label %assert.then.L37, !prof !0

assert.then.L37:                                  ; preds = %0
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %0
  %f2_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 1
  %7 = load i1, ptr %f2_addr, align 1
  br i1 %7, label %assert.exit.L38, label %assert.then.L38, !prof !0

assert.then.L38:                                  ; preds = %assert.exit.L37
  %8 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L38:                                  ; preds = %assert.exit.L37
  %f3_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 2
  %9 = load i1, ptr %f3_addr, align 1
  %10 = xor i1 %9, true
  store i1 %10, ptr %1, align 1
  br i1 %10, label %assert.exit.L39, label %assert.then.L39, !prof !0

assert.then.L39:                                  ; preds = %assert.exit.L38
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L39:                                  ; preds = %assert.exit.L38
  %f4_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 3
  %12 = load i32, ptr %f4_addr, align 4
  %13 = icmp eq i32 %12, 11
  br i1 %13, label %assert.exit.L40, label %assert.then.L40, !prof !0

assert.then.L40:                                  ; preds = %assert.exit.L39
  %14 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L40:                                  ; preds = %assert.exit.L39
  %f5_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 4
  %15 = load i16, ptr %f5_addr, align 2
  %16 = icmp eq i16 %15, 10
  br i1 %16, label %assert.exit.L41, label %assert.then.L41, !prof !0

assert.then.L41:                                  ; preds = %assert.exit.L40
  %17 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L41:                                  ; preds = %assert.exit.L40
  %f6_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 5
  %18 = load i64, ptr %f6_addr, align 8
  %19 = icmp eq i64 %18, 2
  br i1 %19, label %assert.exit.L42, label %assert.then.L42, !prof !0

assert.then.L42:                                  ; preds = %assert.exit.L41
  %20 = call i32 (ptr, ...) @printf(ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L42:                                  ; preds = %assert.exit.L41
  %f7_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 6
  %21 = load i1, ptr %f7_addr, align 1
  br i1 %21, label %assert.exit.L43, label %assert.then.L43, !prof !0

assert.then.L43:                                  ; preds = %assert.exit.L42
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L43:                                  ; preds = %assert.exit.L42
  %f8_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 7
  %23 = load i1, ptr %f8_addr, align 1
  br i1 %23, label %assert.exit.L44, label %assert.then.L44, !prof !0

assert.then.L44:                                  ; preds = %assert.exit.L43
  %24 = call i32 (ptr, ...) @printf(ptr @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L44:                                  ; preds = %assert.exit.L43
  %f9_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 8
  %25 = load i1, ptr %f9_addr, align 1
  br i1 %25, label %assert.exit.L45, label %assert.then.L45, !prof !0

assert.then.L45:                                  ; preds = %assert.exit.L44
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L45:                                  ; preds = %assert.exit.L44
  %f10_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 9
  %27 = load i1, ptr %f10_addr, align 1
  %28 = xor i1 %27, true
  store i1 %28, ptr %2, align 1
  br i1 %28, label %assert.exit.L46, label %assert.then.L46, !prof !0

assert.then.L46:                                  ; preds = %assert.exit.L45
  %29 = call i32 (ptr, ...) @printf(ptr @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L46:                                  ; preds = %assert.exit.L45
  %f11_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 10
  %30 = load i1, ptr %f11_addr, align 1
  br i1 %30, label %assert.exit.L47, label %assert.then.L47, !prof !0

assert.then.L47:                                  ; preds = %assert.exit.L46
  %31 = call i32 (ptr, ...) @printf(ptr @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L47:                                  ; preds = %assert.exit.L46
  %f12_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 11
  %32 = load i1, ptr %f12_addr, align 1
  %33 = xor i1 %32, true
  store i1 %33, ptr %3, align 1
  br i1 %33, label %assert.exit.L48, label %assert.then.L48, !prof !0

assert.then.L48:                                  ; preds = %assert.exit.L47
  %34 = call i32 (ptr, ...) @printf(ptr @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L48:                                  ; preds = %assert.exit.L47
  %f13_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 12
  %35 = load i32, ptr %f13_addr, align 4
  %36 = icmp eq i32 %35, 333
  br i1 %36, label %assert.exit.L49, label %assert.then.L49, !prof !0

assert.then.L49:                                  ; preds = %assert.exit.L48
  %37 = call i32 (ptr, ...) @printf(ptr @anon.string.12)
  call void @exit(i32 1)
  unreachable

assert.exit.L49:                                  ; preds = %assert.exit.L48
  %f14_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 13
  %38 = load i64, ptr %f14_addr, align 8
  %39 = icmp eq i64 %38, 11
  br i1 %39, label %assert.exit.L50, label %assert.then.L50, !prof !0

assert.then.L50:                                  ; preds = %assert.exit.L49
  %40 = call i32 (ptr, ...) @printf(ptr @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %f15_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 14
  %41 = load i8, ptr %f15_addr, align 1
  %42 = icmp eq i8 %41, 63
  br i1 %42, label %assert.exit.L51, label %assert.then.L51, !prof !0

assert.then.L51:                                  ; preds = %assert.exit.L50
  %43 = call i32 (ptr, ...) @printf(ptr @anon.string.14)
  call void @exit(i32 1)
  unreachable

assert.exit.L51:                                  ; preds = %assert.exit.L50
  %f16_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 15
  %44 = load i32, ptr %f16_addr, align 4
  %45 = icmp eq i32 %44, 13
  br i1 %45, label %assert.exit.L52, label %assert.then.L52, !prof !0

assert.then.L52:                                  ; preds = %assert.exit.L51
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L52:                                  ; preds = %assert.exit.L51
  %f17_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 16
  %47 = load i32, ptr %f17_addr, align 4
  %48 = icmp eq i32 %47, 13
  br i1 %48, label %assert.exit.L53, label %assert.then.L53, !prof !0

assert.then.L53:                                  ; preds = %assert.exit.L52
  %49 = call i32 (ptr, ...) @printf(ptr @anon.string.16)
  call void @exit(i32 1)
  unreachable

assert.exit.L53:                                  ; preds = %assert.exit.L52
  %f18_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 17
  %50 = load i32, ptr %f18_addr, align 4
  %51 = icmp eq i32 %50, 14
  br i1 %51, label %assert.exit.L54, label %assert.then.L54, !prof !0

assert.then.L54:                                  ; preds = %assert.exit.L53
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L54:                                  ; preds = %assert.exit.L53
  %f19_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 18
  %53 = load i32, ptr %f19_addr, align 4
  %54 = icmp eq i32 %53, 12
  br i1 %54, label %assert.exit.L55, label %assert.then.L55, !prof !0

assert.then.L55:                                  ; preds = %assert.exit.L54
  %55 = call i32 (ptr, ...) @printf(ptr @anon.string.18)
  call void @exit(i32 1)
  unreachable

assert.exit.L55:                                  ; preds = %assert.exit.L54
  %f20_addr = getelementptr inbounds %struct.Test, ptr %t, i32 0, i32 19
  %56 = load i32, ptr %f20_addr, align 4
  %57 = icmp eq i32 %56, 7
  br i1 %57, label %assert.exit.L56, label %assert.then.L56, !prof !0

assert.then.L56:                                  ; preds = %assert.exit.L55
  %58 = call i32 (ptr, ...) @printf(ptr @anon.string.19)
  call void @exit(i32 1)
  unreachable

assert.exit.L56:                                  ; preds = %assert.exit.L55
  %59 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %60 = load i32, ptr %result, align 4
  ret i32 %60
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

attributes #0 = { norecurse }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { cold noreturn nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
