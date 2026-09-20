; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32, i1, i1, i32, i16, i64, i1, i1, i1, i1, i1, i1, i32, i64, i8, i32, i32, i32, i32, i32 }

@anon.string.0 = private unnamed_addr constant [61 x i8] c"Assertion failed: Condition 't.f1 == 2' evaluated to false.\0A\00", align 4
@stderr = external local_unnamed_addr global ptr, align 8
@anon.string.1 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition 't.f2' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition '!t.f3' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 't.f4 == 11' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f5 == 10s' evaluated to false.\0A\00", align 4
@anon.string.5 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 't.f6 == 2l' evaluated to false.\0A\00", align 4
@anon.string.6 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition 't.f7' evaluated to false.\0A\00", align 4
@anon.string.7 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition 't.f8' evaluated to false.\0A\00", align 4
@anon.string.8 = private unnamed_addr constant [56 x i8] c"Assertion failed: Condition 't.f9' evaluated to false.\0A\00", align 4
@anon.string.9 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition '!t.f10' evaluated to false.\0A\00", align 4
@anon.string.10 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 't.f11' evaluated to false.\0A\00", align 4
@anon.string.11 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition '!t.f12' evaluated to false.\0A\00", align 4
@anon.string.12 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 't.f13 == 333' evaluated to false.\0A\00", align 4
@anon.string.13 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 't.f14 == 11l' evaluated to false.\0A\00", align 4
@anon.string.14 = private unnamed_addr constant [75 x i8] c"Assertion failed: Condition 't.f15 == cast<byte>(63)' evaluated to false.\0A\00", align 4
@anon.string.15 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f16 == 13' evaluated to false.\0A\00", align 4
@anon.string.16 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f17 == 13' evaluated to false.\0A\00", align 4
@anon.string.17 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f18 == 14' evaluated to false.\0A\00", align 4
@anon.string.18 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 't.f19 == 12' evaluated to false.\0A\00", align 4
@anon.string.19 = private unnamed_addr constant [83 x i8] c"Assertion failed: Condition 't.f20 == QualifierType::COMPOSE' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(72) %0) #0 {
  %this = alloca ptr, align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  %6 = load ptr, ptr %this, align 8
  store i32 2, ptr %6, align 4
  %7 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 1
  br i1 false, label %lor.exit.L7C15, label %lor.1.L7C15

lor.1.L7C15:                                      ; preds = %1
  br i1 false, label %lor.exit.L7C15, label %lor.2.L7C15

lor.2.L7C15:                                      ; preds = %lor.1.L7C15
  br label %lor.exit.L7C15

lor.exit.L7C15:                                   ; preds = %lor.2.L7C15, %lor.1.L7C15, %1
  %lor_phi = phi i1 [ false, %1 ], [ false, %lor.1.L7C15 ], [ true, %lor.2.L7C15 ]
  store i1 %lor_phi, ptr %7, align 1
  %8 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 2
  br i1 true, label %land.1.L8C15, label %land.exit.L8C15

land.1.L8C15:                                     ; preds = %lor.exit.L7C15
  br i1 false, label %land.2.L8C15, label %land.exit.L8C15

land.2.L8C15:                                     ; preds = %land.1.L8C15
  br label %land.exit.L8C15

land.exit.L8C15:                                  ; preds = %land.2.L8C15, %land.1.L8C15, %lor.exit.L7C15
  %land_phi = phi i1 [ true, %lor.exit.L7C15 ], [ false, %land.1.L8C15 ], [ true, %land.2.L8C15 ]
  store i1 %land_phi, ptr %8, align 1
  %9 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 3
  store i32 11, ptr %9, align 4
  %10 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 4
  store i16 10, ptr %10, align 2
  %11 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 5
  store i64 2, ptr %11, align 8
  %12 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 6
  store i1 true, ptr %12, align 1
  %13 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 7
  store i1 true, ptr %13, align 1
  %14 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 8
  store i1 true, ptr %14, align 1
  %15 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 9
  store i1 false, ptr %15, align 1
  %16 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 10
  store i1 true, ptr %16, align 1
  %17 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 11
  store i1 false, ptr %17, align 1
  %18 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 12
  store i32 333, ptr %18, align 4
  %19 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 13
  store i64 11, ptr %19, align 8
  %20 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 14
  store i8 63, ptr %20, align 1
  %21 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 15
  store i32 14, ptr %2, align 4
  store i32 13, ptr %21, align 4
  %22 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 16
  store i32 12, ptr %3, align 4
  store i32 13, ptr %22, align 4
  %23 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 17
  store i32 14, ptr %4, align 4
  store i32 14, ptr %23, align 4
  %24 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 18
  store i32 12, ptr %5, align 4
  store i32 12, ptr %24, align 4
  %25 = getelementptr inbounds nuw %struct.Test, ptr %6, i32 0, i32 19
  store i32 0, ptr %25, align 4
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %t = alloca %struct.Test, align 8
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(72) %t)
  %f1.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  %1 = load i32, ptr %f1.addr, align 4
  %2 = icmp eq i32 %1, 2
  br i1 %2, label %assert.exit.L30, label %assert.then.L30, !prof !5

assert.then.L30:                                  ; preds = %0
  %3 = load ptr, ptr @stderr, align 8
  %4 = call i32 (ptr, ptr, ...) @fprintf(ptr %3, ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L30:                                  ; preds = %0
  %f2.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 1
  %5 = load i1, ptr %f2.addr, align 1
  br i1 %5, label %assert.exit.L31, label %assert.then.L31, !prof !5

assert.then.L31:                                  ; preds = %assert.exit.L30
  %6 = load ptr, ptr @stderr, align 8
  %7 = call i32 (ptr, ptr, ...) @fprintf(ptr %6, ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L31:                                  ; preds = %assert.exit.L30
  %f3.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 2
  %8 = load i1, ptr %f3.addr, align 1
  %9 = xor i1 %8, true
  br i1 %9, label %assert.exit.L32, label %assert.then.L32, !prof !5

assert.then.L32:                                  ; preds = %assert.exit.L31
  %10 = load ptr, ptr @stderr, align 8
  %11 = call i32 (ptr, ptr, ...) @fprintf(ptr %10, ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L31
  %f4.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 3
  %12 = load i32, ptr %f4.addr, align 4
  %13 = icmp eq i32 %12, 11
  br i1 %13, label %assert.exit.L33, label %assert.then.L33, !prof !5

assert.then.L33:                                  ; preds = %assert.exit.L32
  %14 = load ptr, ptr @stderr, align 8
  %15 = call i32 (ptr, ptr, ...) @fprintf(ptr %14, ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L33:                                  ; preds = %assert.exit.L32
  %f5.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 4
  %16 = load i16, ptr %f5.addr, align 2
  %17 = icmp eq i16 %16, 10
  br i1 %17, label %assert.exit.L34, label %assert.then.L34, !prof !5

assert.then.L34:                                  ; preds = %assert.exit.L33
  %18 = load ptr, ptr @stderr, align 8
  %19 = call i32 (ptr, ptr, ...) @fprintf(ptr %18, ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L33
  %f6.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 5
  %20 = load i64, ptr %f6.addr, align 8
  %21 = icmp eq i64 %20, 2
  br i1 %21, label %assert.exit.L35, label %assert.then.L35, !prof !5

assert.then.L35:                                  ; preds = %assert.exit.L34
  %22 = load ptr, ptr @stderr, align 8
  %23 = call i32 (ptr, ptr, ...) @fprintf(ptr %22, ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L35:                                  ; preds = %assert.exit.L34
  %f7.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 6
  %24 = load i1, ptr %f7.addr, align 1
  br i1 %24, label %assert.exit.L36, label %assert.then.L36, !prof !5

assert.then.L36:                                  ; preds = %assert.exit.L35
  %25 = load ptr, ptr @stderr, align 8
  %26 = call i32 (ptr, ptr, ...) @fprintf(ptr %25, ptr @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L36:                                  ; preds = %assert.exit.L35
  %f8.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 7
  %27 = load i1, ptr %f8.addr, align 1
  br i1 %27, label %assert.exit.L37, label %assert.then.L37, !prof !5

assert.then.L37:                                  ; preds = %assert.exit.L36
  %28 = load ptr, ptr @stderr, align 8
  %29 = call i32 (ptr, ptr, ...) @fprintf(ptr %28, ptr @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %f9.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 8
  %30 = load i1, ptr %f9.addr, align 1
  br i1 %30, label %assert.exit.L38, label %assert.then.L38, !prof !5

assert.then.L38:                                  ; preds = %assert.exit.L37
  %31 = load ptr, ptr @stderr, align 8
  %32 = call i32 (ptr, ptr, ...) @fprintf(ptr %31, ptr @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L38:                                  ; preds = %assert.exit.L37
  %f10.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 9
  %33 = load i1, ptr %f10.addr, align 1
  %34 = xor i1 %33, true
  br i1 %34, label %assert.exit.L39, label %assert.then.L39, !prof !5

assert.then.L39:                                  ; preds = %assert.exit.L38
  %35 = load ptr, ptr @stderr, align 8
  %36 = call i32 (ptr, ptr, ...) @fprintf(ptr %35, ptr @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L39:                                  ; preds = %assert.exit.L38
  %f11.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 10
  %37 = load i1, ptr %f11.addr, align 1
  br i1 %37, label %assert.exit.L40, label %assert.then.L40, !prof !5

assert.then.L40:                                  ; preds = %assert.exit.L39
  %38 = load ptr, ptr @stderr, align 8
  %39 = call i32 (ptr, ptr, ...) @fprintf(ptr %38, ptr @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L40:                                  ; preds = %assert.exit.L39
  %f12.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 11
  %40 = load i1, ptr %f12.addr, align 1
  %41 = xor i1 %40, true
  br i1 %41, label %assert.exit.L41, label %assert.then.L41, !prof !5

assert.then.L41:                                  ; preds = %assert.exit.L40
  %42 = load ptr, ptr @stderr, align 8
  %43 = call i32 (ptr, ptr, ...) @fprintf(ptr %42, ptr @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L41:                                  ; preds = %assert.exit.L40
  %f13.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 12
  %44 = load i32, ptr %f13.addr, align 4
  %45 = icmp eq i32 %44, 333
  br i1 %45, label %assert.exit.L42, label %assert.then.L42, !prof !5

assert.then.L42:                                  ; preds = %assert.exit.L41
  %46 = load ptr, ptr @stderr, align 8
  %47 = call i32 (ptr, ptr, ...) @fprintf(ptr %46, ptr @anon.string.12)
  call void @exit(i32 1)
  unreachable

assert.exit.L42:                                  ; preds = %assert.exit.L41
  %f14.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 13
  %48 = load i64, ptr %f14.addr, align 8
  %49 = icmp eq i64 %48, 11
  br i1 %49, label %assert.exit.L43, label %assert.then.L43, !prof !5

assert.then.L43:                                  ; preds = %assert.exit.L42
  %50 = load ptr, ptr @stderr, align 8
  %51 = call i32 (ptr, ptr, ...) @fprintf(ptr %50, ptr @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L43:                                  ; preds = %assert.exit.L42
  %f15.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 14
  %52 = load i8, ptr %f15.addr, align 1
  %53 = icmp eq i8 %52, 63
  br i1 %53, label %assert.exit.L44, label %assert.then.L44, !prof !5

assert.then.L44:                                  ; preds = %assert.exit.L43
  %54 = load ptr, ptr @stderr, align 8
  %55 = call i32 (ptr, ptr, ...) @fprintf(ptr %54, ptr @anon.string.14)
  call void @exit(i32 1)
  unreachable

assert.exit.L44:                                  ; preds = %assert.exit.L43
  %f16.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 15
  %56 = load i32, ptr %f16.addr, align 4
  %57 = icmp eq i32 %56, 13
  br i1 %57, label %assert.exit.L45, label %assert.then.L45, !prof !5

assert.then.L45:                                  ; preds = %assert.exit.L44
  %58 = load ptr, ptr @stderr, align 8
  %59 = call i32 (ptr, ptr, ...) @fprintf(ptr %58, ptr @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L45:                                  ; preds = %assert.exit.L44
  %f17.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 16
  %60 = load i32, ptr %f17.addr, align 4
  %61 = icmp eq i32 %60, 13
  br i1 %61, label %assert.exit.L46, label %assert.then.L46, !prof !5

assert.then.L46:                                  ; preds = %assert.exit.L45
  %62 = load ptr, ptr @stderr, align 8
  %63 = call i32 (ptr, ptr, ...) @fprintf(ptr %62, ptr @anon.string.16)
  call void @exit(i32 1)
  unreachable

assert.exit.L46:                                  ; preds = %assert.exit.L45
  %f18.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 17
  %64 = load i32, ptr %f18.addr, align 4
  %65 = icmp eq i32 %64, 14
  br i1 %65, label %assert.exit.L47, label %assert.then.L47, !prof !5

assert.then.L47:                                  ; preds = %assert.exit.L46
  %66 = load ptr, ptr @stderr, align 8
  %67 = call i32 (ptr, ptr, ...) @fprintf(ptr %66, ptr @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L47:                                  ; preds = %assert.exit.L46
  %f19.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 18
  %68 = load i32, ptr %f19.addr, align 4
  %69 = icmp eq i32 %68, 12
  br i1 %69, label %assert.exit.L48, label %assert.then.L48, !prof !5

assert.then.L48:                                  ; preds = %assert.exit.L47
  %70 = load ptr, ptr @stderr, align 8
  %71 = call i32 (ptr, ptr, ...) @fprintf(ptr %70, ptr @anon.string.18)
  call void @exit(i32 1)
  unreachable

assert.exit.L48:                                  ; preds = %assert.exit.L47
  %f20.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 19
  %72 = load i32, ptr %f20.addr, align 4
  %73 = icmp eq i32 %72, 0
  br i1 %73, label %assert.exit.L49, label %assert.then.L49, !prof !5

assert.then.L49:                                  ; preds = %assert.exit.L48
  %74 = load ptr, ptr @stderr, align 8
  %75 = call i32 (ptr, ptr, ...) @fprintf(ptr %74, ptr @anon.string.19)
  call void @exit(i32 1)
  unreachable

assert.exit.L49:                                  ; preds = %assert.exit.L48
  %76 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i32 0
}

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

attributes #0 = { mustprogress noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree }
attributes #3 = { cold noreturn nounwind }
attributes #4 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
