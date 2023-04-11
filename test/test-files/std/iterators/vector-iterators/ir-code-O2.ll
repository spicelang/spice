; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__int__Vector__intptr_long_long_int = type { ptr, i64, i64, i32 }
%"__int__VectorIterator__std/data/vector::Vector<int>ref_long" = type { ptr, i64 }

@anon.string.0 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.getSize() == 3' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'it.next() == 4321' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'it.next() == 9876' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 9876' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.hasNext()' evaluated to false.\00", align 1
@anon.string.13 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == -99' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!it.hasNext()' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %vi = alloca %__int__Vector__intptr_long_long_int, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %it = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  call void @_mp__Vector_int__void__ctor(ptr nonnull %vi) #2
  store i32 123, ptr %1, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %1) #2
  store i32 4321, ptr %2, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %2) #2
  store i32 9876, ptr %3, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %3) #2
  %6 = call i64 @_mf__Vector_int__long__getSize(ptr nonnull %vi) #2
  %7 = icmp eq i64 %6, 3
  br i1 %7, label %assert.exit.L9, label %assert.then.L9, !prof !0

assert.then.L9:                                   ; preds = %0
  %8 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.0)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L9:                                   ; preds = %0
  %9 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr nonnull %vi) #2
  %.fca.0.extract = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %9, 0
  %.fca.1.extract = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %9, 1
  %.fca.1.gep = getelementptr inbounds %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, i64 0, i32 1
  store ptr %.fca.0.extract, ptr %it, align 8
  store i64 %.fca.1.extract, ptr %.fca.1.gep, align 8
  %10 = call i1 @_mf__VectorIterator_int__bool__hasNext(ptr nonnull %it) #2
  br i1 %10, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L9
  %11 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L9
  %12 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %13 = load i32, ptr %12, align 4
  %14 = icmp eq i32 %13, 123
  br i1 %14, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L13
  %15 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %16 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %17 = load i32, ptr %16, align 4
  %18 = icmp eq i32 %17, 123
  br i1 %18, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L14
  %19 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L14
  %20 = call ptr @_mf__VectorIterator_int__intref__next(ptr nonnull %it) #2
  %21 = load i32, ptr %20, align 4
  %22 = icmp eq i32 %21, 4321
  br i1 %22, label %assert.exit.L16, label %assert.then.L16, !prof !0

assert.then.L16:                                  ; preds = %assert.exit.L15
  %23 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.4)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L16:                                  ; preds = %assert.exit.L15
  %24 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %25 = load i32, ptr %24, align 4
  %26 = icmp eq i32 %25, 4321
  br i1 %26, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L16
  %27 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.5)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L16
  %28 = call i1 @_mf__VectorIterator_int__bool__hasNext(ptr nonnull %it) #2
  br i1 %28, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L17
  %29 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L17
  %30 = call ptr @_mf__VectorIterator_int__intref__next(ptr nonnull %it) #2
  %31 = load i32, ptr %30, align 4
  %32 = icmp eq i32 %31, 9876
  br i1 %32, label %assert.exit.L19, label %assert.then.L19, !prof !0

assert.then.L19:                                  ; preds = %assert.exit.L18
  %33 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L18
  %34 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %35 = load i32, ptr %34, align 4
  %36 = icmp eq i32 %35, 9876
  br i1 %36, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L19
  %37 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L19
  %38 = call i1 @_mf__VectorIterator_int__bool__hasNext(ptr nonnull %it) #2
  br i1 %38, label %assert.then.L21, label %assert.exit.L21, !prof !1

assert.then.L21:                                  ; preds = %assert.exit.L20
  %39 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.14)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L20
  store i32 321, ptr %4, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %4) #2
  store i32 -99, ptr %5, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %5) #2
  %40 = call i1 @_mf__VectorIterator_int__bool__hasNext(ptr nonnull %it) #2
  br i1 %40, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %assert.exit.L21
  %41 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L21
  call void @"_p__void__void__op.minusequal__VectorIterator<int>ref_int"(ptr nonnull %it, i32 2) #2
  %42 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %43 = load i32, ptr %42, align 4
  %44 = icmp eq i32 %43, 123
  br i1 %44, label %assert.exit.L30, label %assert.then.L30, !prof !0

assert.then.L30:                                  ; preds = %assert.exit.L26
  %45 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L26
  %46 = call i1 @_mf__VectorIterator_int__bool__hasNext(ptr nonnull %it) #2
  br i1 %46, label %assert.exit.L31, label %assert.then.L31, !prof !0

assert.then.L31:                                  ; preds = %assert.exit.L30
  %47 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L31:                                  ; preds = %assert.exit.L30
  call void @"_p__void__void__op.plusequal__VectorIterator<int>ref_int"(ptr nonnull %it, i32 4) #2
  %48 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %49 = load i32, ptr %48, align 4
  %50 = icmp eq i32 %49, -99
  br i1 %50, label %assert.exit.L33, label %assert.then.L33, !prof !0

assert.then.L33:                                  ; preds = %assert.exit.L31
  %51 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L33:                                  ; preds = %assert.exit.L31
  %52 = call i1 @_mf__VectorIterator_int__bool__hasNext(ptr nonnull %it) #2
  br i1 %52, label %assert.then.L34, label %assert.exit.L34, !prof !1

assert.then.L34:                                  ; preds = %assert.exit.L33
  %53 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.14)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L33
  %54 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare void @_mp__Vector_int__void__ctor(ptr) local_unnamed_addr

declare void @_mp__Vector_int__void__pushBack__intref(ptr, ptr) local_unnamed_addr

declare i64 @_mf__Vector_int__long__getSize(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @exit(i32) local_unnamed_addr

declare %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr) local_unnamed_addr

declare i1 @_mf__VectorIterator_int__bool__hasNext(ptr) local_unnamed_addr

declare ptr @_mf__VectorIterator_int__intref__get(ptr) local_unnamed_addr

declare ptr @_mf__VectorIterator_int__intref__next(ptr) local_unnamed_addr

declare void @"_p__void__void__op.minusequal__VectorIterator<int>ref_int"(ptr, i32) local_unnamed_addr

declare void @"_p__void__void__op.plusequal__VectorIterator<int>ref_int"(ptr, i32) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
