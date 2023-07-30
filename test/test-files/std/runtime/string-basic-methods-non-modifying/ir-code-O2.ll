; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.String = type { ptr, i64, i64 }

@anon.string.2 = private unnamed_addr constant [7 x i8] c"Hello!\00", align 1
@anon.string.3 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.4 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 's1.isEmpty()' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!s2.isEmpty()' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 's3.getLength() == 6' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 's4.getLength() == 12' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 's3.getCapacity() == 12' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 's4.getCapacity() == 24' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 's2.isFull()' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition '!s4.isFull()' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [4 x i8] c"ell\00", align 1
@anon.string.13 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 's4.find(\22ell\22) == 1' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [5 x i8] c"Wort\00", align 1
@anon.string.15 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's4.find(\22Wort\22) == -1' evaluated to false.\00", align 1
@anon.string.16 = private unnamed_addr constant [2 x i8] c"H\00", align 1
@anon.string.17 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 's4.find(\22H\22) == 0' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [2 x i8] c"!\00", align 1
@anon.string.19 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 's4.find(\22!\22) == 11' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [2 x i8] c" \00", align 1
@anon.string.21 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 's4.find(\22 \22, 12) == -1' evaluated to false.\00", align 1
@anon.string.22 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@anon.string.23 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition '!s4.contains(\22abc\22)' evaluated to false.\00", align 1
@anon.string.25 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 's4.contains(\22Hello\22)' evaluated to false.\00", align 1
@anon.string.27 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 's4.contains(\22World!\22)' evaluated to false.\00", align 1
@anon.string.28 = private unnamed_addr constant [4 x i8] c"o W\00", align 1
@anon.string.29 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 's4.contains(\22o W\22)' evaluated to false.\00", align 1
@anon.string.30 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.31 = private unnamed_addr constant [74 x i8] c"Assertion failed: Condition 'sub.getRaw() == \22Hello\22' evaluated to false.\00", align 1
@anon.string.32 = private unnamed_addr constant [3 x i8] c"o \00", align 1
@anon.string.33 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'sub.getRaw() == \22o \22' evaluated to false.\00", align 1
@anon.string.34 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.35 = private unnamed_addr constant [75 x i8] c"Assertion failed: Condition 'sub.getRaw() == \22World!\22' evaluated to false.\00", align 1
@anon.string.36 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.37 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'sub.getRaw() == \22\22' evaluated to false.\00", align 1
@anon.string.38 = private unnamed_addr constant [11 x i8] c"llo World!\00", align 1
@anon.string.39 = private unnamed_addr constant [79 x i8] c"Assertion failed: Condition 'sub.getRaw() == \22llo World!\22' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %s1 = alloca %struct.String, align 8
  %s2 = alloca %struct.String, align 8
  %s3 = alloca %struct.String, align 8
  %s4 = alloca %struct.String, align 8
  %sub = alloca %struct.String, align 8
  call void @_ZN6String4ctorEPc(ptr nonnull %s1, ptr nonnull @anon.string.36) #3
  call void @_ZN6String4ctorEPc(ptr nonnull %s2, ptr nonnull @anon.string.30) #3
  call void @_ZN6String4ctorEPc(ptr nonnull %s3, ptr nonnull @anon.string.2) #3
  call void @_ZN6String4ctorEPc(ptr nonnull %s4, ptr nonnull @anon.string.3) #3
  %1 = call i1 @_ZN6String7isEmptyEv(ptr nonnull %s1) #3
  br i1 %1, label %assert.exit.L8, label %assert.then.L8, !prof !0

assert.then.L8:                                   ; preds = %0
  %2 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L8:                                   ; preds = %0
  %3 = call i1 @_ZN6String7isEmptyEv(ptr nonnull %s2) #3
  br i1 %3, label %assert.then.L9, label %assert.exit.L9, !prof !1

assert.then.L9:                                   ; preds = %assert.exit.L8
  %4 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %assert.exit.L8
  %5 = call i64 @_ZN6String9getLengthEv(ptr nonnull %s3) #3
  %6 = icmp eq i64 %5, 6
  br i1 %6, label %assert.exit.L10, label %assert.then.L10, !prof !0

assert.then.L10:                                  ; preds = %assert.exit.L9
  %7 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L9
  %8 = call i64 @_ZN6String9getLengthEv(ptr nonnull %s4) #3
  %9 = icmp eq i64 %8, 12
  br i1 %9, label %assert.exit.L11, label %assert.then.L11, !prof !0

assert.then.L11:                                  ; preds = %assert.exit.L10
  %10 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L11:                                  ; preds = %assert.exit.L10
  %11 = call i64 @_ZN6String11getCapacityEv(ptr nonnull %s3) #3
  %12 = icmp eq i64 %11, 12
  br i1 %12, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %assert.exit.L11
  %13 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %assert.exit.L11
  %14 = call i64 @_ZN6String11getCapacityEv(ptr nonnull %s4) #3
  %15 = icmp eq i64 %14, 24
  br i1 %15, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L12
  %16 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L12
  %17 = call i1 @_ZN6String6isFullEv(ptr nonnull %s2) #3
  br i1 %17, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L13
  %18 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %19 = call i1 @_ZN6String6isFullEv(ptr nonnull %s4) #3
  br i1 %19, label %assert.then.L15, label %assert.exit.L15, !prof !1

assert.then.L15:                                  ; preds = %assert.exit.L14
  %20 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L14
  %21 = call i64 @_ZN6String4findEPc(ptr nonnull %s4, ptr nonnull @anon.string.12) #3
  %22 = icmp eq i64 %21, 1
  br i1 %22, label %assert.exit.L16, label %assert.then.L16, !prof !0

assert.then.L16:                                  ; preds = %assert.exit.L15
  %23 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L16:                                  ; preds = %assert.exit.L15
  %24 = call i64 @_ZN6String4findEPc(ptr nonnull %s4, ptr nonnull @anon.string.14) #3
  %25 = icmp eq i64 %24, -1
  br i1 %25, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L16
  %26 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L16
  %27 = call i64 @_ZN6String4findEPc(ptr nonnull %s4, ptr nonnull @anon.string.16) #3
  %28 = icmp eq i64 %27, 0
  br i1 %28, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L17
  %29 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L17
  %30 = call i64 @_ZN6String4findEPc(ptr nonnull %s4, ptr nonnull @anon.string.18) #3
  %31 = icmp eq i64 %30, 11
  br i1 %31, label %assert.exit.L19, label %assert.then.L19, !prof !0

assert.then.L19:                                  ; preds = %assert.exit.L18
  %32 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.19)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L18
  %33 = call i64 @_ZN6String4findEPcj(ptr nonnull %s4, ptr nonnull @anon.string.20, i32 12) #3
  %34 = icmp eq i64 %33, -1
  br i1 %34, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L19
  %35 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.21)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L19
  %36 = call i1 @_ZN6String8containsEPc(ptr nonnull %s4, ptr nonnull @anon.string.22) #3
  br i1 %36, label %assert.then.L21, label %assert.exit.L21, !prof !1

assert.then.L21:                                  ; preds = %assert.exit.L20
  %37 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.23)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L20
  %38 = call i1 @_ZN6String8containsEPc(ptr nonnull %s4, ptr nonnull @anon.string.30) #3
  br i1 %38, label %assert.exit.L22, label %assert.then.L22, !prof !0

assert.then.L22:                                  ; preds = %assert.exit.L21
  %39 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.25)
  call void @exit(i32 1)
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L21
  %40 = call i1 @_ZN6String8containsEPc(ptr nonnull %s4, ptr nonnull @anon.string.34) #3
  br i1 %40, label %assert.exit.L23, label %assert.then.L23, !prof !0

assert.then.L23:                                  ; preds = %assert.exit.L22
  %41 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.27)
  call void @exit(i32 1)
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L22
  %42 = call i1 @_ZN6String8containsEPc(ptr nonnull %s4, ptr nonnull @anon.string.28) #3
  br i1 %42, label %assert.exit.L24, label %assert.then.L24, !prof !0

assert.then.L24:                                  ; preds = %assert.exit.L23
  %43 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.29)
  call void @exit(i32 1)
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L23
  %44 = call %struct.String @_ZN6String12getSubstringEjl(ptr nonnull %s4, i32 0, i64 5) #3
  %.fca.0.extract19 = extractvalue %struct.String %44, 0
  %.fca.1.extract20 = extractvalue %struct.String %44, 1
  %.fca.1.gep = getelementptr inbounds %struct.String, ptr %sub, i64 0, i32 1
  %.fca.2.extract21 = extractvalue %struct.String %44, 2
  %.fca.2.gep = getelementptr inbounds %struct.String, ptr %sub, i64 0, i32 2
  store ptr %.fca.0.extract19, ptr %sub, align 8
  store i64 %.fca.1.extract20, ptr %.fca.1.gep, align 8
  store i64 %.fca.2.extract21, ptr %.fca.2.gep, align 8
  %45 = call ptr @_ZN6String6getRawEv(ptr nonnull %sub) #3
  %46 = call i1 @_Z10isRawEqualPcPc(ptr %45, ptr nonnull @anon.string.30) #3
  br i1 %46, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %assert.exit.L24
  %47 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.31)
  call void @exit(i32 1)
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L24
  %48 = call %struct.String @_ZN6String12getSubstringEml(ptr nonnull %s4, i64 4, i64 2) #3
  %.fca.0.extract13 = extractvalue %struct.String %48, 0
  %.fca.1.extract14 = extractvalue %struct.String %48, 1
  %.fca.2.extract15 = extractvalue %struct.String %48, 2
  store ptr %.fca.0.extract13, ptr %sub, align 8
  store i64 %.fca.1.extract14, ptr %.fca.1.gep, align 8
  store i64 %.fca.2.extract15, ptr %.fca.2.gep, align 8
  %49 = call ptr @_ZN6String6getRawEv(ptr nonnull %sub) #3
  %50 = call i1 @_Z10isRawEqualPcPc(ptr %49, ptr nonnull @anon.string.32) #3
  br i1 %50, label %assert.exit.L28, label %assert.then.L28, !prof !0

assert.then.L28:                                  ; preds = %assert.exit.L26
  %51 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.33)
  call void @exit(i32 1)
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L26
  %52 = call %struct.String @_ZN6String12getSubstringEj(ptr nonnull %s4, i32 6) #3
  %.fca.0.extract7 = extractvalue %struct.String %52, 0
  %.fca.1.extract8 = extractvalue %struct.String %52, 1
  %.fca.2.extract9 = extractvalue %struct.String %52, 2
  store ptr %.fca.0.extract7, ptr %sub, align 8
  store i64 %.fca.1.extract8, ptr %.fca.1.gep, align 8
  store i64 %.fca.2.extract9, ptr %.fca.2.gep, align 8
  %53 = call ptr @_ZN6String6getRawEv(ptr nonnull %sub) #3
  %54 = call i1 @_Z10isRawEqualPcPc(ptr %53, ptr nonnull @anon.string.34) #3
  br i1 %54, label %assert.exit.L30, label %assert.then.L30, !prof !0

assert.then.L30:                                  ; preds = %assert.exit.L28
  %55 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.35)
  call void @exit(i32 1)
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L28
  %56 = call %struct.String @_ZN6String12getSubstringEjl(ptr nonnull %s4, i32 2, i64 0) #3
  %.fca.0.extract1 = extractvalue %struct.String %56, 0
  %.fca.1.extract2 = extractvalue %struct.String %56, 1
  %.fca.2.extract3 = extractvalue %struct.String %56, 2
  store ptr %.fca.0.extract1, ptr %sub, align 8
  store i64 %.fca.1.extract2, ptr %.fca.1.gep, align 8
  store i64 %.fca.2.extract3, ptr %.fca.2.gep, align 8
  %57 = call ptr @_ZN6String6getRawEv(ptr nonnull %sub) #3
  %58 = call i1 @_Z10isRawEqualPcPc(ptr %57, ptr nonnull @anon.string.36) #3
  br i1 %58, label %assert.exit.L32, label %assert.then.L32, !prof !0

assert.then.L32:                                  ; preds = %assert.exit.L30
  %59 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.37)
  call void @exit(i32 1)
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L30
  %60 = call %struct.String @_ZN6String12getSubstringEml(ptr nonnull %s4, i64 2, i64 12) #3
  %.fca.0.extract = extractvalue %struct.String %60, 0
  %.fca.1.extract = extractvalue %struct.String %60, 1
  %.fca.2.extract = extractvalue %struct.String %60, 2
  store ptr %.fca.0.extract, ptr %sub, align 8
  store i64 %.fca.1.extract, ptr %.fca.1.gep, align 8
  store i64 %.fca.2.extract, ptr %.fca.2.gep, align 8
  %61 = call ptr @_ZN6String6getRawEv(ptr nonnull %sub) #3
  %62 = call i1 @_Z10isRawEqualPcPc(ptr %61, ptr nonnull @anon.string.38) #3
  br i1 %62, label %assert.exit.L34, label %assert.then.L34, !prof !0

assert.then.L34:                                  ; preds = %assert.exit.L32
  %63 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.39)
  call void @exit(i32 1)
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L32
  %64 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  call void @_ZN6String4dtorEv(ptr nonnull %sub) #3
  call void @_ZN6String4dtorEv(ptr nonnull %s4) #3
  call void @_ZN6String4dtorEv(ptr nonnull %s3) #3
  call void @_ZN6String4dtorEv(ptr nonnull %s2) #3
  call void @_ZN6String4dtorEv(ptr nonnull %s1) #3
  ret i32 0
}

declare void @_ZN6String4ctorEPc(ptr, ptr) local_unnamed_addr

declare i1 @_ZN6String7isEmptyEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) local_unnamed_addr #2

declare i64 @_ZN6String9getLengthEv(ptr) local_unnamed_addr

declare i64 @_ZN6String11getCapacityEv(ptr) local_unnamed_addr

declare i1 @_ZN6String6isFullEv(ptr) local_unnamed_addr

declare i64 @_ZN6String4findEPc(ptr, ptr) local_unnamed_addr

declare i64 @_ZN6String4findEPcj(ptr, ptr, i32) local_unnamed_addr

declare i1 @_ZN6String8containsEPc(ptr, ptr) local_unnamed_addr

declare %struct.String @_ZN6String12getSubstringEjl(ptr, i32, i64) local_unnamed_addr

declare ptr @_ZN6String6getRawEv(ptr) local_unnamed_addr

declare i1 @_Z10isRawEqualPcPc(ptr, ptr) local_unnamed_addr

declare %struct.String @_ZN6String12getSubstringEml(ptr, i64, i64) local_unnamed_addr

declare %struct.String @_ZN6String12getSubstringEj(ptr, i32) local_unnamed_addr

declare void @_ZN6String4dtorEv(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
