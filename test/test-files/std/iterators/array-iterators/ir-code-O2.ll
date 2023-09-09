; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.ArrayIterator = type { ptr, i64, i64 }
%struct.Pair = type { i64, ptr }

@anon.string.5 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'pair.getFirst() == 2' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [75 x i8] c"Assertion failed: Condition 'pair.getSecond() == 9876' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == -99' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!it.isValid()' evaluated to false.\00", align 1
@anon.string.13 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 'a[0] == 123' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[1] == 4321' evaluated to false.\00", align 1
@anon.string.15 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[2] == 9876' evaluated to false.\00", align 1
@anon.string.17 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[1] == 4322' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[2] == 9877' evaluated to false.\00", align 1
@anon.string.19 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 'a[0] == 124' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[1] == 4323' evaluated to false.\00", align 1
@anon.string.21 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[2] == 9879' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %a = alloca [5 x i32], align 4
  %it = alloca %struct.ArrayIterator, align 8
  %pair = alloca %struct.Pair, align 8
  %1 = alloca %struct.ArrayIterator, align 8
  %2 = alloca %struct.ArrayIterator, align 8
  %3 = alloca %struct.ArrayIterator, align 8
  store i32 123, ptr %a, align 4
  %.fca.1.gep50 = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 1
  store i32 4321, ptr %.fca.1.gep50, align 4
  %.fca.2.gep51 = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 2
  store i32 9876, ptr %.fca.2.gep51, align 4
  %.fca.3.gep = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 3
  store i32 321, ptr %.fca.3.gep, align 4
  %.fca.4.gep = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 4
  store i32 -99, ptr %.fca.4.gep, align 4
  %4 = call %struct.ArrayIterator @_Z7iteratePim(ptr nonnull %a, i64 5) #3
  %.fca.0.extract25 = extractvalue %struct.ArrayIterator %4, 0
  %.fca.1.extract27 = extractvalue %struct.ArrayIterator %4, 1
  %.fca.1.gep28 = getelementptr inbounds %struct.ArrayIterator, ptr %it, i64 0, i32 1
  %.fca.2.extract29 = extractvalue %struct.ArrayIterator %4, 2
  %.fca.2.gep30 = getelementptr inbounds %struct.ArrayIterator, ptr %it, i64 0, i32 2
  store ptr %.fca.0.extract25, ptr %it, align 8
  store i64 %.fca.1.extract27, ptr %.fca.1.gep28, align 8
  store i64 %.fca.2.extract29, ptr %.fca.2.gep30, align 8
  %5 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %it) #3
  br i1 %5, label %assert.exit.L11, label %assert.then.L11, !prof !0

assert.then.L11:                                  ; preds = %0
  %6 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L11:                                  ; preds = %0
  %7 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #3
  %8 = load i32, ptr %7, align 4
  %9 = icmp eq i32 %8, 123
  br i1 %9, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %assert.exit.L11
  %10 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %assert.exit.L11
  %11 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #3
  %12 = load i32, ptr %11, align 4
  %13 = icmp eq i32 %12, 123
  br i1 %13, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L12
  %14 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L12
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %it) #3
  %15 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #3
  %16 = load i32, ptr %15, align 4
  %17 = icmp eq i32 %16, 4321
  br i1 %17, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L13
  %18 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L13
  %19 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %it) #3
  br i1 %19, label %assert.exit.L16, label %assert.then.L16, !prof !0

assert.then.L16:                                  ; preds = %assert.exit.L15
  %20 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L16:                                  ; preds = %assert.exit.L15
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %it) #3
  %21 = call %struct.Pair @_ZN13ArrayIteratorIiE6getIdxEv(ptr nonnull %it) #3
  %.fca.0.extract17 = extractvalue %struct.Pair %21, 0
  %.fca.1.extract19 = extractvalue %struct.Pair %21, 1
  %.fca.1.gep20 = getelementptr inbounds %struct.Pair, ptr %pair, i64 0, i32 1
  store i64 %.fca.0.extract17, ptr %pair, align 8
  store ptr %.fca.1.extract19, ptr %.fca.1.gep20, align 8
  %22 = call ptr @_ZN4PairImRiE8getFirstEv(ptr nonnull %pair) #3
  %23 = load i64, ptr %22, align 8
  %24 = icmp eq i64 %23, 2
  br i1 %24, label %assert.exit.L19, label %assert.then.L19, !prof !0

assert.then.L19:                                  ; preds = %assert.exit.L16
  %25 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L16
  %26 = call ptr @_ZN4PairImRiE9getSecondEv(ptr nonnull %pair) #3
  %27 = load i32, ptr %26, align 4
  %28 = icmp eq i32 %27, 9876
  br i1 %28, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L19
  %29 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L19
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %it) #3
  call void @_Z13op.minusequalR13ArrayIteratorIiEi(ptr nonnull %it, i32 3) #3
  %30 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #3
  %31 = load i32, ptr %30, align 4
  %32 = icmp eq i32 %31, 123
  br i1 %32, label %assert.exit.L25, label %assert.then.L25, !prof !0

assert.then.L25:                                  ; preds = %assert.exit.L20
  %33 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L25:                                  ; preds = %assert.exit.L20
  %34 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %it) #3
  br i1 %34, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %assert.exit.L25
  %35 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L25
  call void @_Z16op.plusplus.postR13ArrayIteratorIiE(ptr nonnull %it) #3
  %36 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #3
  %37 = load i32, ptr %36, align 4
  %38 = icmp eq i32 %37, 4321
  br i1 %38, label %assert.exit.L28, label %assert.then.L28, !prof !0

assert.then.L28:                                  ; preds = %assert.exit.L26
  %39 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L26
  call void @_Z18op.minusminus.postR13ArrayIteratorIiE(ptr nonnull %it) #3
  %40 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #3
  %41 = load i32, ptr %40, align 4
  %42 = icmp eq i32 %41, 123
  br i1 %42, label %assert.exit.L30, label %assert.then.L30, !prof !0

assert.then.L30:                                  ; preds = %assert.exit.L28
  %43 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L28
  call void @_Z12op.plusequalR13ArrayIteratorIiEi(ptr nonnull %it, i32 4) #3
  %44 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #3
  %45 = load i32, ptr %44, align 4
  %46 = icmp eq i32 %45, -99
  br i1 %46, label %assert.exit.L32, label %assert.then.L32, !prof !0

assert.then.L32:                                  ; preds = %assert.exit.L30
  %47 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L30
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %it) #3
  %48 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %it) #3
  br i1 %48, label %assert.then.L34, label %assert.exit.L34, !prof !1

assert.then.L34:                                  ; preds = %assert.exit.L32
  %49 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1)
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L32
  %50 = call %struct.ArrayIterator @_Z7iteratePim(ptr nonnull %a, i64 5) #3
  %.fca.0.extract11 = extractvalue %struct.ArrayIterator %50, 0
  store ptr %.fca.0.extract11, ptr %1, align 8
  %.fca.1.extract13 = extractvalue %struct.ArrayIterator %50, 1
  %.fca.1.gep14 = getelementptr inbounds %struct.ArrayIterator, ptr %1, i64 0, i32 1
  store i64 %.fca.1.extract13, ptr %.fca.1.gep14, align 8
  %.fca.2.extract15 = extractvalue %struct.ArrayIterator %50, 2
  %.fca.2.gep16 = getelementptr inbounds %struct.ArrayIterator, ptr %1, i64 0, i32 2
  store i64 %.fca.2.extract15, ptr %.fca.2.gep16, align 8
  %51 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %1) #3
  br i1 %51, label %foreach.body.L37, label %foreach.exit.L37

foreach.body.L37:                                 ; preds = %assert.exit.L34, %foreach.body.L37
  %52 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %1) #3
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %1) #3
  %53 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %1) #3
  br i1 %53, label %foreach.body.L37, label %foreach.exit.L37

foreach.exit.L37:                                 ; preds = %foreach.body.L37, %assert.exit.L34
  %54 = load i32, ptr %a, align 4
  %55 = icmp eq i32 %54, 123
  br i1 %55, label %assert.exit.L40, label %assert.then.L40, !prof !0

assert.then.L40:                                  ; preds = %foreach.exit.L37
  %56 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L40:                                  ; preds = %foreach.exit.L37
  %57 = load i32, ptr %.fca.1.gep50, align 4
  %58 = icmp eq i32 %57, 4321
  br i1 %58, label %assert.exit.L41, label %assert.then.L41, !prof !0

assert.then.L41:                                  ; preds = %assert.exit.L40
  %59 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.14)
  call void @exit(i32 1)
  unreachable

assert.exit.L41:                                  ; preds = %assert.exit.L40
  %60 = load i32, ptr %.fca.2.gep51, align 4
  %61 = icmp eq i32 %60, 9876
  br i1 %61, label %assert.exit.L42, label %assert.then.L42, !prof !0

assert.then.L42:                                  ; preds = %assert.exit.L41
  %62 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L42:                                  ; preds = %assert.exit.L41
  %63 = call %struct.ArrayIterator @_Z7iteratePim(ptr nonnull %a, i64 5) #3
  %.fca.0.extract5 = extractvalue %struct.ArrayIterator %63, 0
  store ptr %.fca.0.extract5, ptr %2, align 8
  %.fca.1.extract7 = extractvalue %struct.ArrayIterator %63, 1
  %.fca.1.gep8 = getelementptr inbounds %struct.ArrayIterator, ptr %2, i64 0, i32 1
  store i64 %.fca.1.extract7, ptr %.fca.1.gep8, align 8
  %.fca.2.extract9 = extractvalue %struct.ArrayIterator %63, 2
  %.fca.2.gep10 = getelementptr inbounds %struct.ArrayIterator, ptr %2, i64 0, i32 2
  store i64 %.fca.2.extract9, ptr %.fca.2.gep10, align 8
  %64 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %2) #3
  br i1 %64, label %foreach.body.L45, label %foreach.exit.L45

foreach.body.L45:                                 ; preds = %assert.exit.L42, %foreach.body.L45
  %65 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %2) #3
  %66 = load i32, ptr %65, align 4
  %67 = add i32 %66, 1
  store i32 %67, ptr %65, align 4
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %2) #3
  %68 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %2) #3
  br i1 %68, label %foreach.body.L45, label %foreach.exit.L45

foreach.exit.L45:                                 ; preds = %foreach.body.L45, %assert.exit.L42
  %69 = load i32, ptr %a, align 4
  %70 = icmp eq i32 %69, 124
  br i1 %70, label %assert.exit.L48, label %assert.then.L48, !prof !0

assert.then.L48:                                  ; preds = %foreach.exit.L45
  %71 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.19)
  call void @exit(i32 1)
  unreachable

assert.exit.L48:                                  ; preds = %foreach.exit.L45
  %72 = load i32, ptr %.fca.1.gep50, align 4
  %73 = icmp eq i32 %72, 4322
  br i1 %73, label %assert.exit.L49, label %assert.then.L49, !prof !0

assert.then.L49:                                  ; preds = %assert.exit.L48
  %74 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L49:                                  ; preds = %assert.exit.L48
  %75 = load i32, ptr %.fca.2.gep51, align 4
  %76 = icmp eq i32 %75, 9877
  br i1 %76, label %assert.exit.L50, label %assert.then.L50, !prof !0

assert.then.L50:                                  ; preds = %assert.exit.L49
  %77 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.18)
  call void @exit(i32 1)
  unreachable

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %78 = call %struct.ArrayIterator @_Z7iteratePim(ptr nonnull %a, i64 5) #3
  %.fca.0.extract = extractvalue %struct.ArrayIterator %78, 0
  store ptr %.fca.0.extract, ptr %3, align 8
  %.fca.1.extract = extractvalue %struct.ArrayIterator %78, 1
  %.fca.1.gep = getelementptr inbounds %struct.ArrayIterator, ptr %3, i64 0, i32 1
  store i64 %.fca.1.extract, ptr %.fca.1.gep, align 8
  %.fca.2.extract = extractvalue %struct.ArrayIterator %78, 2
  %.fca.2.gep = getelementptr inbounds %struct.ArrayIterator, ptr %3, i64 0, i32 2
  store i64 %.fca.2.extract, ptr %.fca.2.gep, align 8
  %79 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %3) #3
  br i1 %79, label %foreach.body.L52, label %foreach.exit.L52

foreach.body.L52:                                 ; preds = %assert.exit.L50, %foreach.body.L52
  %pair3 = call %struct.Pair @_ZN13ArrayIteratorIiE6getIdxEv(ptr nonnull %3) #3
  %pair3.fca.0.extract = extractvalue %struct.Pair %pair3, 0
  %pair3.fca.1.extract = extractvalue %struct.Pair %pair3, 1
  %80 = trunc i64 %pair3.fca.0.extract to i32
  %81 = load i32, ptr %pair3.fca.1.extract, align 4
  %82 = add i32 %81, %80
  store i32 %82, ptr %pair3.fca.1.extract, align 4
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %3) #3
  %83 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %3) #3
  br i1 %83, label %foreach.body.L52, label %foreach.exit.L52

foreach.exit.L52:                                 ; preds = %foreach.body.L52, %assert.exit.L50
  %84 = load i32, ptr %a, align 4
  %85 = icmp eq i32 %84, 124
  br i1 %85, label %assert.exit.L55, label %assert.then.L55, !prof !0

assert.then.L55:                                  ; preds = %foreach.exit.L52
  %86 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.19)
  call void @exit(i32 1)
  unreachable

assert.exit.L55:                                  ; preds = %foreach.exit.L52
  %87 = load i32, ptr %.fca.1.gep50, align 4
  %88 = icmp eq i32 %87, 4323
  br i1 %88, label %assert.exit.L56, label %assert.then.L56, !prof !0

assert.then.L56:                                  ; preds = %assert.exit.L55
  %89 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.20)
  call void @exit(i32 1)
  unreachable

assert.exit.L56:                                  ; preds = %assert.exit.L55
  %90 = load i32, ptr %.fca.2.gep51, align 4
  %91 = icmp eq i32 %90, 9879
  br i1 %91, label %assert.exit.L57, label %assert.then.L57, !prof !0

assert.then.L57:                                  ; preds = %assert.exit.L56
  %92 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.21)
  call void @exit(i32 1)
  unreachable

assert.exit.L57:                                  ; preds = %assert.exit.L56
  %93 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare %struct.ArrayIterator @_Z7iteratePim(ptr, i64) local_unnamed_addr

declare i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) local_unnamed_addr #2

declare ptr @_ZN13ArrayIteratorIiE3getEv(ptr) local_unnamed_addr

declare void @_ZN13ArrayIteratorIiE4nextEv(ptr) local_unnamed_addr

declare %struct.Pair @_ZN13ArrayIteratorIiE6getIdxEv(ptr) local_unnamed_addr

declare ptr @_ZN4PairImRiE8getFirstEv(ptr) local_unnamed_addr

declare ptr @_ZN4PairImRiE9getSecondEv(ptr) local_unnamed_addr

declare void @_Z13op.minusequalR13ArrayIteratorIiEi(ptr, i32) local_unnamed_addr

declare void @_Z16op.plusplus.postR13ArrayIteratorIiE(ptr) local_unnamed_addr

declare void @_Z18op.minusminus.postR13ArrayIteratorIiE(ptr) local_unnamed_addr

declare void @_Z12op.plusequalR13ArrayIteratorIiEi(ptr, i32) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
