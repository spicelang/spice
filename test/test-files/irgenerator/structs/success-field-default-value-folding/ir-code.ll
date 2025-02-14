; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32, i1, i1, i32, i16, i64, i1, i1, i1, i1, i1, i1, i32, i64, i8, i32, i32, i32, i32, i32 }

@anon.string.0 = private unnamed_addr constant [61 x i8] c"Assertion failed: Condition 't.f1 == 2' evaluated to false.\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition 't.f2' evaluated to false.\0A\00", align 1
@anon.string.2 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition '!t.f3' evaluated to false.\0A\00", align 1
@anon.string.3 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 't.f4 == 11' evaluated to false.\0A\00", align 1
@anon.string.4 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f5 == 10s' evaluated to false.\0A\00", align 1
@anon.string.5 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 't.f6 == 2l' evaluated to false.\0A\00", align 1
@anon.string.6 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition 't.f7' evaluated to false.\0A\00", align 1
@anon.string.7 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition 't.f8' evaluated to false.\0A\00", align 1
@anon.string.8 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition 't.f9' evaluated to false.\0A\00", align 1
@anon.string.9 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition '!t.f10' evaluated to false.\0A\00", align 1
@anon.string.10 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 't.f11' evaluated to false.\0A\00", align 1
@anon.string.11 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition '!t.f12' evaluated to false.\0A\00", align 1
@anon.string.12 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 't.f13 == 333' evaluated to false.\0A\00", align 1
@anon.string.13 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 't.f14 == 11l' evaluated to false.\0A\00", align 1
@anon.string.14 = private unnamed_addr constant [75 x i8] c"Assertion failed: Condition 't.f15 == cast<byte>(63)' evaluated to false.\0A\00", align 1
@anon.string.15 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f16 == 13' evaluated to false.\0A\00", align 1
@anon.string.16 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f17 == 13' evaluated to false.\0A\00", align 1
@anon.string.17 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f18 == 14' evaluated to false.\0A\00", align 1
@anon.string.18 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f19 == 12' evaluated to false.\0A\00", align 1
@anon.string.19 = private unnamed_addr constant [83 x i8] c"Assertion failed: Condition 't.f20 == QualifierType::COMPOSE' evaluated to false.\0A\00", align 1
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
  %7 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 0
  store i32 2, ptr %7, align 4
  %8 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 1
  br i1 false, label %lor.exit.L14C15, label %lor.1.L14C15

lor.1.L14C15:                                     ; preds = %1
  br i1 false, label %lor.exit.L14C15, label %lor.2.L14C15

lor.2.L14C15:                                     ; preds = %lor.1.L14C15
  br label %lor.exit.L14C15

lor.exit.L14C15:                                  ; preds = %lor.2.L14C15, %lor.1.L14C15, %1
  %lor_phi = phi i1 [ false, %1 ], [ false, %lor.1.L14C15 ], [ true, %lor.2.L14C15 ]
  store i1 %lor_phi, ptr %8, align 1
  %9 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 2
  br i1 true, label %land.1.L15C15, label %land.exit.L15C15

land.1.L15C15:                                    ; preds = %lor.exit.L14C15
  br i1 false, label %land.2.L15C15, label %land.exit.L15C15

land.2.L15C15:                                    ; preds = %land.1.L15C15
  br label %land.exit.L15C15

land.exit.L15C15:                                 ; preds = %land.2.L15C15, %land.1.L15C15, %lor.exit.L14C15
  %land_phi = phi i1 [ true, %lor.exit.L14C15 ], [ false, %land.1.L15C15 ], [ true, %land.2.L15C15 ]
  store i1 %land_phi, ptr %9, align 1
  %10 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 3
  store i32 11, ptr %10, align 4
  %11 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 4
  store i16 10, ptr %11, align 2
  %12 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 5
  store i64 2, ptr %12, align 8
  %13 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 6
  store i1 true, ptr %13, align 1
  %14 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 7
  store i1 true, ptr %14, align 1
  %15 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 8
  store i1 true, ptr %15, align 1
  %16 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 9
  store i1 false, ptr %16, align 1
  %17 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 10
  store i1 true, ptr %17, align 1
  %18 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 11
  store i1 false, ptr %18, align 1
  %19 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 12
  store i32 333, ptr %19, align 4
  %20 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 13
  store i64 11, ptr %20, align 8
  %21 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 14
  store i8 63, ptr %21, align 1
  %22 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 15
  store i32 14, ptr %2, align 4
  store i32 13, ptr %22, align 4
  %23 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 16
  store i32 12, ptr %3, align 4
  store i32 13, ptr %23, align 4
  %24 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 17
  store i32 14, ptr %4, align 4
  store i32 14, ptr %24, align 4
  %25 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 18
  store i32 12, ptr %5, align 4
  store i32 12, ptr %25, align 4
  %26 = getelementptr inbounds %struct.Test, ptr %6, i64 0, i32 19
  store i32 7, ptr %26, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %t = alloca %struct.Test, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN4Test4ctorEv(ptr %t)
  %f1.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  %1 = load i32, ptr %f1.addr, align 4
  %2 = icmp eq i32 %1, 2
  br i1 %2, label %assert.exit.L37, label %assert.then.L37, !prof !5

assert.then.L37:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %0
  %f2.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 1
  %4 = load i1, ptr %f2.addr, align 1
  br i1 %4, label %assert.exit.L38, label %assert.then.L38, !prof !5

assert.then.L38:                                  ; preds = %assert.exit.L37
  %5 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L38:                                  ; preds = %assert.exit.L37
  %f3.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 2
  %6 = load i1, ptr %f3.addr, align 1
  %7 = xor i1 %6, true
  br i1 %7, label %assert.exit.L39, label %assert.then.L39, !prof !5

assert.then.L39:                                  ; preds = %assert.exit.L38
  %8 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L39:                                  ; preds = %assert.exit.L38
  %f4.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 3
  %9 = load i32, ptr %f4.addr, align 4
  %10 = icmp eq i32 %9, 11
  br i1 %10, label %assert.exit.L40, label %assert.then.L40, !prof !5

assert.then.L40:                                  ; preds = %assert.exit.L39
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L40:                                  ; preds = %assert.exit.L39
  %f5.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 4
  %12 = load i16, ptr %f5.addr, align 2
  %13 = icmp eq i16 %12, 10
  br i1 %13, label %assert.exit.L41, label %assert.then.L41, !prof !5

assert.then.L41:                                  ; preds = %assert.exit.L40
  %14 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L41:                                  ; preds = %assert.exit.L40
  %f6.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 5
  %15 = load i64, ptr %f6.addr, align 8
  %16 = icmp eq i64 %15, 2
  br i1 %16, label %assert.exit.L42, label %assert.then.L42, !prof !5

assert.then.L42:                                  ; preds = %assert.exit.L41
  %17 = call i32 (ptr, ...) @printf(ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L42:                                  ; preds = %assert.exit.L41
  %f7.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 6
  %18 = load i1, ptr %f7.addr, align 1
  br i1 %18, label %assert.exit.L43, label %assert.then.L43, !prof !5

assert.then.L43:                                  ; preds = %assert.exit.L42
  %19 = call i32 (ptr, ...) @printf(ptr @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L43:                                  ; preds = %assert.exit.L42
  %f8.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 7
  %20 = load i1, ptr %f8.addr, align 1
  br i1 %20, label %assert.exit.L44, label %assert.then.L44, !prof !5

assert.then.L44:                                  ; preds = %assert.exit.L43
  %21 = call i32 (ptr, ...) @printf(ptr @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L44:                                  ; preds = %assert.exit.L43
  %f9.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 8
  %22 = load i1, ptr %f9.addr, align 1
  br i1 %22, label %assert.exit.L45, label %assert.then.L45, !prof !5

assert.then.L45:                                  ; preds = %assert.exit.L44
  %23 = call i32 (ptr, ...) @printf(ptr @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L45:                                  ; preds = %assert.exit.L44
  %f10.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 9
  %24 = load i1, ptr %f10.addr, align 1
  %25 = xor i1 %24, true
  br i1 %25, label %assert.exit.L46, label %assert.then.L46, !prof !5

assert.then.L46:                                  ; preds = %assert.exit.L45
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L46:                                  ; preds = %assert.exit.L45
  %f11.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 10
  %27 = load i1, ptr %f11.addr, align 1
  br i1 %27, label %assert.exit.L47, label %assert.then.L47, !prof !5

assert.then.L47:                                  ; preds = %assert.exit.L46
  %28 = call i32 (ptr, ...) @printf(ptr @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L47:                                  ; preds = %assert.exit.L46
  %f12.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 11
  %29 = load i1, ptr %f12.addr, align 1
  %30 = xor i1 %29, true
  br i1 %30, label %assert.exit.L48, label %assert.then.L48, !prof !5

assert.then.L48:                                  ; preds = %assert.exit.L47
  %31 = call i32 (ptr, ...) @printf(ptr @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L48:                                  ; preds = %assert.exit.L47
  %f13.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 12
  %32 = load i32, ptr %f13.addr, align 4
  %33 = icmp eq i32 %32, 333
  br i1 %33, label %assert.exit.L49, label %assert.then.L49, !prof !5

assert.then.L49:                                  ; preds = %assert.exit.L48
  %34 = call i32 (ptr, ...) @printf(ptr @anon.string.12)
  call void @exit(i32 1)
  unreachable

assert.exit.L49:                                  ; preds = %assert.exit.L48
  %f14.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 13
  %35 = load i64, ptr %f14.addr, align 8
  %36 = icmp eq i64 %35, 11
  br i1 %36, label %assert.exit.L50, label %assert.then.L50, !prof !5

assert.then.L50:                                  ; preds = %assert.exit.L49
  %37 = call i32 (ptr, ...) @printf(ptr @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %f15.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 14
  %38 = load i8, ptr %f15.addr, align 1
  %39 = icmp eq i8 %38, 63
  br i1 %39, label %assert.exit.L51, label %assert.then.L51, !prof !5

assert.then.L51:                                  ; preds = %assert.exit.L50
  %40 = call i32 (ptr, ...) @printf(ptr @anon.string.14)
  call void @exit(i32 1)
  unreachable

assert.exit.L51:                                  ; preds = %assert.exit.L50
  %f16.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 15
  %41 = load i32, ptr %f16.addr, align 4
  %42 = icmp eq i32 %41, 13
  br i1 %42, label %assert.exit.L52, label %assert.then.L52, !prof !5

assert.then.L52:                                  ; preds = %assert.exit.L51
  %43 = call i32 (ptr, ...) @printf(ptr @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L52:                                  ; preds = %assert.exit.L51
  %f17.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 16
  %44 = load i32, ptr %f17.addr, align 4
  %45 = icmp eq i32 %44, 13
  br i1 %45, label %assert.exit.L53, label %assert.then.L53, !prof !5

assert.then.L53:                                  ; preds = %assert.exit.L52
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.16)
  call void @exit(i32 1)
  unreachable

assert.exit.L53:                                  ; preds = %assert.exit.L52
  %f18.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 17
  %47 = load i32, ptr %f18.addr, align 4
  %48 = icmp eq i32 %47, 14
  br i1 %48, label %assert.exit.L54, label %assert.then.L54, !prof !5

assert.then.L54:                                  ; preds = %assert.exit.L53
  %49 = call i32 (ptr, ...) @printf(ptr @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L54:                                  ; preds = %assert.exit.L53
  %f19.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 18
  %50 = load i32, ptr %f19.addr, align 4
  %51 = icmp eq i32 %50, 12
  br i1 %51, label %assert.exit.L55, label %assert.then.L55, !prof !5

assert.then.L55:                                  ; preds = %assert.exit.L54
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.18)
  call void @exit(i32 1)
  unreachable

assert.exit.L55:                                  ; preds = %assert.exit.L54
  %f20.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 19
  %53 = load i32, ptr %f20.addr, align 4
  %54 = icmp eq i32 %53, 7
  br i1 %54, label %assert.exit.L56, label %assert.then.L56, !prof !5

assert.then.L56:                                  ; preds = %assert.exit.L55
  %55 = call i32 (ptr, ...) @printf(ptr @anon.string.19)
  call void @exit(i32 1)
  unreachable

assert.exit.L56:                                  ; preds = %assert.exit.L55
  %56 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %57 = load i32, ptr %result, align 4
  ret i32 %57
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

attributes #0 = { norecurse }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { cold noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 2000, i32 1}
