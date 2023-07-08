; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@RAND_COUNT = private unnamed_addr global i32 0
@printf.str.1 = private unnamed_addr constant [22 x i8] c"\E2\95\AD\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\95\AE\00", align 1
@printf.str.4 = private unnamed_addr constant [22 x i8] c"\E2\94\9C\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\A4\00", align 1
@printf.str.7 = private unnamed_addr constant [22 x i8] c"\E2\95\B0\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\94\80\E2\95\AF\00", align 1
@printf.str.9 = private unnamed_addr constant [5 x i8] c"%s:\0A\00", align 1
@printf.str.10 = private unnamed_addr constant [13 x i8] c"\E2\94\82  %d  \E2\94\82\00", align 1
@anon.string.0 = private unnamed_addr constant [14 x i8] c"Initial state\00", align 1
@anon.string.1 = private unnamed_addr constant [16 x i8] c"Next generation\00", align 1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, argmem: none, inaccessiblemem: none)
define private fastcc i32 @_f__void__int__genFakeRandInt() unnamed_addr #0 {
  %1 = load i32, ptr @RAND_COUNT, align 4
  %2 = add i32 %1, 1
  store i32 %2, ptr @RAND_COUNT, align 4
  %3 = srem i32 %2, 2
  ret i32 %3
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: nofree nounwind
define private fastcc void @_p__void__void__printGeneration__string_intarrayarray(ptr %0, [10 x [10 x i32]] %1) unnamed_addr #1 {
for.body.L33.peel.next:
  %matrix = alloca [10 x [10 x i32]], align 4
  %.fca.0.0.extract = extractvalue [10 x [10 x i32]] %1, 0, 0
  store i32 %.fca.0.0.extract, ptr %matrix, align 4
  %.fca.0.1.extract = extractvalue [10 x [10 x i32]] %1, 0, 1
  %.fca.0.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 1
  store i32 %.fca.0.1.extract, ptr %.fca.0.1.gep, align 4
  %.fca.0.2.extract = extractvalue [10 x [10 x i32]] %1, 0, 2
  %.fca.0.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 2
  store i32 %.fca.0.2.extract, ptr %.fca.0.2.gep, align 4
  %.fca.0.3.extract = extractvalue [10 x [10 x i32]] %1, 0, 3
  %.fca.0.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 3
  store i32 %.fca.0.3.extract, ptr %.fca.0.3.gep, align 4
  %.fca.0.4.extract = extractvalue [10 x [10 x i32]] %1, 0, 4
  %.fca.0.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 4
  store i32 %.fca.0.4.extract, ptr %.fca.0.4.gep, align 4
  %.fca.0.5.extract = extractvalue [10 x [10 x i32]] %1, 0, 5
  %.fca.0.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 5
  store i32 %.fca.0.5.extract, ptr %.fca.0.5.gep, align 4
  %.fca.0.6.extract = extractvalue [10 x [10 x i32]] %1, 0, 6
  %.fca.0.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 6
  store i32 %.fca.0.6.extract, ptr %.fca.0.6.gep, align 4
  %.fca.0.7.extract = extractvalue [10 x [10 x i32]] %1, 0, 7
  %.fca.0.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 7
  store i32 %.fca.0.7.extract, ptr %.fca.0.7.gep, align 4
  %.fca.0.8.extract = extractvalue [10 x [10 x i32]] %1, 0, 8
  %.fca.0.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 8
  store i32 %.fca.0.8.extract, ptr %.fca.0.8.gep, align 4
  %.fca.0.9.extract = extractvalue [10 x [10 x i32]] %1, 0, 9
  %.fca.0.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 9
  store i32 %.fca.0.9.extract, ptr %.fca.0.9.gep, align 4
  %.fca.1.0.extract = extractvalue [10 x [10 x i32]] %1, 1, 0
  %.fca.1.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 0
  store i32 %.fca.1.0.extract, ptr %.fca.1.0.gep, align 4
  %.fca.1.1.extract = extractvalue [10 x [10 x i32]] %1, 1, 1
  %.fca.1.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 1
  store i32 %.fca.1.1.extract, ptr %.fca.1.1.gep, align 4
  %.fca.1.2.extract = extractvalue [10 x [10 x i32]] %1, 1, 2
  %.fca.1.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 2
  store i32 %.fca.1.2.extract, ptr %.fca.1.2.gep, align 4
  %.fca.1.3.extract = extractvalue [10 x [10 x i32]] %1, 1, 3
  %.fca.1.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 3
  store i32 %.fca.1.3.extract, ptr %.fca.1.3.gep, align 4
  %.fca.1.4.extract = extractvalue [10 x [10 x i32]] %1, 1, 4
  %.fca.1.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 4
  store i32 %.fca.1.4.extract, ptr %.fca.1.4.gep, align 4
  %.fca.1.5.extract = extractvalue [10 x [10 x i32]] %1, 1, 5
  %.fca.1.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 5
  store i32 %.fca.1.5.extract, ptr %.fca.1.5.gep, align 4
  %.fca.1.6.extract = extractvalue [10 x [10 x i32]] %1, 1, 6
  %.fca.1.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 6
  store i32 %.fca.1.6.extract, ptr %.fca.1.6.gep, align 4
  %.fca.1.7.extract = extractvalue [10 x [10 x i32]] %1, 1, 7
  %.fca.1.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 7
  store i32 %.fca.1.7.extract, ptr %.fca.1.7.gep, align 4
  %.fca.1.8.extract = extractvalue [10 x [10 x i32]] %1, 1, 8
  %.fca.1.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 8
  store i32 %.fca.1.8.extract, ptr %.fca.1.8.gep, align 4
  %.fca.1.9.extract = extractvalue [10 x [10 x i32]] %1, 1, 9
  %.fca.1.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 9
  store i32 %.fca.1.9.extract, ptr %.fca.1.9.gep, align 4
  %.fca.2.0.extract = extractvalue [10 x [10 x i32]] %1, 2, 0
  %.fca.2.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 0
  store i32 %.fca.2.0.extract, ptr %.fca.2.0.gep, align 4
  %.fca.2.1.extract = extractvalue [10 x [10 x i32]] %1, 2, 1
  %.fca.2.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 1
  store i32 %.fca.2.1.extract, ptr %.fca.2.1.gep, align 4
  %.fca.2.2.extract = extractvalue [10 x [10 x i32]] %1, 2, 2
  %.fca.2.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 2
  store i32 %.fca.2.2.extract, ptr %.fca.2.2.gep, align 4
  %.fca.2.3.extract = extractvalue [10 x [10 x i32]] %1, 2, 3
  %.fca.2.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 3
  store i32 %.fca.2.3.extract, ptr %.fca.2.3.gep, align 4
  %.fca.2.4.extract = extractvalue [10 x [10 x i32]] %1, 2, 4
  %.fca.2.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 4
  store i32 %.fca.2.4.extract, ptr %.fca.2.4.gep, align 4
  %.fca.2.5.extract = extractvalue [10 x [10 x i32]] %1, 2, 5
  %.fca.2.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 5
  store i32 %.fca.2.5.extract, ptr %.fca.2.5.gep, align 4
  %.fca.2.6.extract = extractvalue [10 x [10 x i32]] %1, 2, 6
  %.fca.2.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 6
  store i32 %.fca.2.6.extract, ptr %.fca.2.6.gep, align 4
  %.fca.2.7.extract = extractvalue [10 x [10 x i32]] %1, 2, 7
  %.fca.2.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 7
  store i32 %.fca.2.7.extract, ptr %.fca.2.7.gep, align 4
  %.fca.2.8.extract = extractvalue [10 x [10 x i32]] %1, 2, 8
  %.fca.2.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 8
  store i32 %.fca.2.8.extract, ptr %.fca.2.8.gep, align 4
  %.fca.2.9.extract = extractvalue [10 x [10 x i32]] %1, 2, 9
  %.fca.2.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 9
  store i32 %.fca.2.9.extract, ptr %.fca.2.9.gep, align 4
  %.fca.3.0.extract = extractvalue [10 x [10 x i32]] %1, 3, 0
  %.fca.3.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 0
  store i32 %.fca.3.0.extract, ptr %.fca.3.0.gep, align 4
  %.fca.3.1.extract = extractvalue [10 x [10 x i32]] %1, 3, 1
  %.fca.3.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 1
  store i32 %.fca.3.1.extract, ptr %.fca.3.1.gep, align 4
  %.fca.3.2.extract = extractvalue [10 x [10 x i32]] %1, 3, 2
  %.fca.3.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 2
  store i32 %.fca.3.2.extract, ptr %.fca.3.2.gep, align 4
  %.fca.3.3.extract = extractvalue [10 x [10 x i32]] %1, 3, 3
  %.fca.3.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 3
  store i32 %.fca.3.3.extract, ptr %.fca.3.3.gep, align 4
  %.fca.3.4.extract = extractvalue [10 x [10 x i32]] %1, 3, 4
  %.fca.3.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 4
  store i32 %.fca.3.4.extract, ptr %.fca.3.4.gep, align 4
  %.fca.3.5.extract = extractvalue [10 x [10 x i32]] %1, 3, 5
  %.fca.3.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 5
  store i32 %.fca.3.5.extract, ptr %.fca.3.5.gep, align 4
  %.fca.3.6.extract = extractvalue [10 x [10 x i32]] %1, 3, 6
  %.fca.3.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 6
  store i32 %.fca.3.6.extract, ptr %.fca.3.6.gep, align 4
  %.fca.3.7.extract = extractvalue [10 x [10 x i32]] %1, 3, 7
  %.fca.3.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 7
  store i32 %.fca.3.7.extract, ptr %.fca.3.7.gep, align 4
  %.fca.3.8.extract = extractvalue [10 x [10 x i32]] %1, 3, 8
  %.fca.3.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 8
  store i32 %.fca.3.8.extract, ptr %.fca.3.8.gep, align 4
  %.fca.3.9.extract = extractvalue [10 x [10 x i32]] %1, 3, 9
  %.fca.3.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 9
  store i32 %.fca.3.9.extract, ptr %.fca.3.9.gep, align 4
  %.fca.4.0.extract = extractvalue [10 x [10 x i32]] %1, 4, 0
  %.fca.4.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 0
  store i32 %.fca.4.0.extract, ptr %.fca.4.0.gep, align 4
  %.fca.4.1.extract = extractvalue [10 x [10 x i32]] %1, 4, 1
  %.fca.4.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 1
  store i32 %.fca.4.1.extract, ptr %.fca.4.1.gep, align 4
  %.fca.4.2.extract = extractvalue [10 x [10 x i32]] %1, 4, 2
  %.fca.4.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 2
  store i32 %.fca.4.2.extract, ptr %.fca.4.2.gep, align 4
  %.fca.4.3.extract = extractvalue [10 x [10 x i32]] %1, 4, 3
  %.fca.4.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 3
  store i32 %.fca.4.3.extract, ptr %.fca.4.3.gep, align 4
  %.fca.4.4.extract = extractvalue [10 x [10 x i32]] %1, 4, 4
  %.fca.4.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 4
  store i32 %.fca.4.4.extract, ptr %.fca.4.4.gep, align 4
  %.fca.4.5.extract = extractvalue [10 x [10 x i32]] %1, 4, 5
  %.fca.4.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 5
  store i32 %.fca.4.5.extract, ptr %.fca.4.5.gep, align 4
  %.fca.4.6.extract = extractvalue [10 x [10 x i32]] %1, 4, 6
  %.fca.4.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 6
  store i32 %.fca.4.6.extract, ptr %.fca.4.6.gep, align 4
  %.fca.4.7.extract = extractvalue [10 x [10 x i32]] %1, 4, 7
  %.fca.4.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 7
  store i32 %.fca.4.7.extract, ptr %.fca.4.7.gep, align 4
  %.fca.4.8.extract = extractvalue [10 x [10 x i32]] %1, 4, 8
  %.fca.4.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 8
  store i32 %.fca.4.8.extract, ptr %.fca.4.8.gep, align 4
  %.fca.4.9.extract = extractvalue [10 x [10 x i32]] %1, 4, 9
  %.fca.4.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 9
  store i32 %.fca.4.9.extract, ptr %.fca.4.9.gep, align 4
  %.fca.5.0.extract = extractvalue [10 x [10 x i32]] %1, 5, 0
  %.fca.5.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 0
  store i32 %.fca.5.0.extract, ptr %.fca.5.0.gep, align 4
  %.fca.5.1.extract = extractvalue [10 x [10 x i32]] %1, 5, 1
  %.fca.5.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 1
  store i32 %.fca.5.1.extract, ptr %.fca.5.1.gep, align 4
  %.fca.5.2.extract = extractvalue [10 x [10 x i32]] %1, 5, 2
  %.fca.5.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 2
  store i32 %.fca.5.2.extract, ptr %.fca.5.2.gep, align 4
  %.fca.5.3.extract = extractvalue [10 x [10 x i32]] %1, 5, 3
  %.fca.5.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 3
  store i32 %.fca.5.3.extract, ptr %.fca.5.3.gep, align 4
  %.fca.5.4.extract = extractvalue [10 x [10 x i32]] %1, 5, 4
  %.fca.5.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 4
  store i32 %.fca.5.4.extract, ptr %.fca.5.4.gep, align 4
  %.fca.5.5.extract = extractvalue [10 x [10 x i32]] %1, 5, 5
  %.fca.5.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 5
  store i32 %.fca.5.5.extract, ptr %.fca.5.5.gep, align 4
  %.fca.5.6.extract = extractvalue [10 x [10 x i32]] %1, 5, 6
  %.fca.5.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 6
  store i32 %.fca.5.6.extract, ptr %.fca.5.6.gep, align 4
  %.fca.5.7.extract = extractvalue [10 x [10 x i32]] %1, 5, 7
  %.fca.5.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 7
  store i32 %.fca.5.7.extract, ptr %.fca.5.7.gep, align 4
  %.fca.5.8.extract = extractvalue [10 x [10 x i32]] %1, 5, 8
  %.fca.5.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 8
  store i32 %.fca.5.8.extract, ptr %.fca.5.8.gep, align 4
  %.fca.5.9.extract = extractvalue [10 x [10 x i32]] %1, 5, 9
  %.fca.5.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 9
  store i32 %.fca.5.9.extract, ptr %.fca.5.9.gep, align 4
  %.fca.6.0.extract = extractvalue [10 x [10 x i32]] %1, 6, 0
  %.fca.6.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 0
  store i32 %.fca.6.0.extract, ptr %.fca.6.0.gep, align 4
  %.fca.6.1.extract = extractvalue [10 x [10 x i32]] %1, 6, 1
  %.fca.6.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 1
  store i32 %.fca.6.1.extract, ptr %.fca.6.1.gep, align 4
  %.fca.6.2.extract = extractvalue [10 x [10 x i32]] %1, 6, 2
  %.fca.6.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 2
  store i32 %.fca.6.2.extract, ptr %.fca.6.2.gep, align 4
  %.fca.6.3.extract = extractvalue [10 x [10 x i32]] %1, 6, 3
  %.fca.6.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 3
  store i32 %.fca.6.3.extract, ptr %.fca.6.3.gep, align 4
  %.fca.6.4.extract = extractvalue [10 x [10 x i32]] %1, 6, 4
  %.fca.6.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 4
  store i32 %.fca.6.4.extract, ptr %.fca.6.4.gep, align 4
  %.fca.6.5.extract = extractvalue [10 x [10 x i32]] %1, 6, 5
  %.fca.6.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 5
  store i32 %.fca.6.5.extract, ptr %.fca.6.5.gep, align 4
  %.fca.6.6.extract = extractvalue [10 x [10 x i32]] %1, 6, 6
  %.fca.6.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 6
  store i32 %.fca.6.6.extract, ptr %.fca.6.6.gep, align 4
  %.fca.6.7.extract = extractvalue [10 x [10 x i32]] %1, 6, 7
  %.fca.6.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 7
  store i32 %.fca.6.7.extract, ptr %.fca.6.7.gep, align 4
  %.fca.6.8.extract = extractvalue [10 x [10 x i32]] %1, 6, 8
  %.fca.6.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 8
  store i32 %.fca.6.8.extract, ptr %.fca.6.8.gep, align 4
  %.fca.6.9.extract = extractvalue [10 x [10 x i32]] %1, 6, 9
  %.fca.6.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 9
  store i32 %.fca.6.9.extract, ptr %.fca.6.9.gep, align 4
  %.fca.7.0.extract = extractvalue [10 x [10 x i32]] %1, 7, 0
  %.fca.7.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 0
  store i32 %.fca.7.0.extract, ptr %.fca.7.0.gep, align 4
  %.fca.7.1.extract = extractvalue [10 x [10 x i32]] %1, 7, 1
  %.fca.7.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 1
  store i32 %.fca.7.1.extract, ptr %.fca.7.1.gep, align 4
  %.fca.7.2.extract = extractvalue [10 x [10 x i32]] %1, 7, 2
  %.fca.7.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 2
  store i32 %.fca.7.2.extract, ptr %.fca.7.2.gep, align 4
  %.fca.7.3.extract = extractvalue [10 x [10 x i32]] %1, 7, 3
  %.fca.7.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 3
  store i32 %.fca.7.3.extract, ptr %.fca.7.3.gep, align 4
  %.fca.7.4.extract = extractvalue [10 x [10 x i32]] %1, 7, 4
  %.fca.7.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 4
  store i32 %.fca.7.4.extract, ptr %.fca.7.4.gep, align 4
  %.fca.7.5.extract = extractvalue [10 x [10 x i32]] %1, 7, 5
  %.fca.7.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 5
  store i32 %.fca.7.5.extract, ptr %.fca.7.5.gep, align 4
  %.fca.7.6.extract = extractvalue [10 x [10 x i32]] %1, 7, 6
  %.fca.7.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 6
  store i32 %.fca.7.6.extract, ptr %.fca.7.6.gep, align 4
  %.fca.7.7.extract = extractvalue [10 x [10 x i32]] %1, 7, 7
  %.fca.7.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 7
  store i32 %.fca.7.7.extract, ptr %.fca.7.7.gep, align 4
  %.fca.7.8.extract = extractvalue [10 x [10 x i32]] %1, 7, 8
  %.fca.7.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 8
  store i32 %.fca.7.8.extract, ptr %.fca.7.8.gep, align 4
  %.fca.7.9.extract = extractvalue [10 x [10 x i32]] %1, 7, 9
  %.fca.7.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 9
  store i32 %.fca.7.9.extract, ptr %.fca.7.9.gep, align 4
  %.fca.8.0.extract = extractvalue [10 x [10 x i32]] %1, 8, 0
  %.fca.8.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 0
  store i32 %.fca.8.0.extract, ptr %.fca.8.0.gep, align 4
  %.fca.8.1.extract = extractvalue [10 x [10 x i32]] %1, 8, 1
  %.fca.8.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 1
  store i32 %.fca.8.1.extract, ptr %.fca.8.1.gep, align 4
  %.fca.8.2.extract = extractvalue [10 x [10 x i32]] %1, 8, 2
  %.fca.8.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 2
  store i32 %.fca.8.2.extract, ptr %.fca.8.2.gep, align 4
  %.fca.8.3.extract = extractvalue [10 x [10 x i32]] %1, 8, 3
  %.fca.8.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 3
  store i32 %.fca.8.3.extract, ptr %.fca.8.3.gep, align 4
  %.fca.8.4.extract = extractvalue [10 x [10 x i32]] %1, 8, 4
  %.fca.8.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 4
  store i32 %.fca.8.4.extract, ptr %.fca.8.4.gep, align 4
  %.fca.8.5.extract = extractvalue [10 x [10 x i32]] %1, 8, 5
  %.fca.8.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 5
  store i32 %.fca.8.5.extract, ptr %.fca.8.5.gep, align 4
  %.fca.8.6.extract = extractvalue [10 x [10 x i32]] %1, 8, 6
  %.fca.8.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 6
  store i32 %.fca.8.6.extract, ptr %.fca.8.6.gep, align 4
  %.fca.8.7.extract = extractvalue [10 x [10 x i32]] %1, 8, 7
  %.fca.8.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 7
  store i32 %.fca.8.7.extract, ptr %.fca.8.7.gep, align 4
  %.fca.8.8.extract = extractvalue [10 x [10 x i32]] %1, 8, 8
  %.fca.8.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 8
  store i32 %.fca.8.8.extract, ptr %.fca.8.8.gep, align 4
  %.fca.8.9.extract = extractvalue [10 x [10 x i32]] %1, 8, 9
  %.fca.8.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 9
  store i32 %.fca.8.9.extract, ptr %.fca.8.9.gep, align 4
  %.fca.9.0.extract = extractvalue [10 x [10 x i32]] %1, 9, 0
  %.fca.9.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 0
  store i32 %.fca.9.0.extract, ptr %.fca.9.0.gep, align 4
  %.fca.9.1.extract = extractvalue [10 x [10 x i32]] %1, 9, 1
  %.fca.9.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 1
  store i32 %.fca.9.1.extract, ptr %.fca.9.1.gep, align 4
  %.fca.9.2.extract = extractvalue [10 x [10 x i32]] %1, 9, 2
  %.fca.9.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 2
  store i32 %.fca.9.2.extract, ptr %.fca.9.2.gep, align 4
  %.fca.9.3.extract = extractvalue [10 x [10 x i32]] %1, 9, 3
  %.fca.9.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 3
  store i32 %.fca.9.3.extract, ptr %.fca.9.3.gep, align 4
  %.fca.9.4.extract = extractvalue [10 x [10 x i32]] %1, 9, 4
  %.fca.9.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 4
  store i32 %.fca.9.4.extract, ptr %.fca.9.4.gep, align 4
  %.fca.9.5.extract = extractvalue [10 x [10 x i32]] %1, 9, 5
  %.fca.9.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 5
  store i32 %.fca.9.5.extract, ptr %.fca.9.5.gep, align 4
  %.fca.9.6.extract = extractvalue [10 x [10 x i32]] %1, 9, 6
  %.fca.9.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 6
  store i32 %.fca.9.6.extract, ptr %.fca.9.6.gep, align 4
  %.fca.9.7.extract = extractvalue [10 x [10 x i32]] %1, 9, 7
  %.fca.9.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 7
  store i32 %.fca.9.7.extract, ptr %.fca.9.7.gep, align 4
  %.fca.9.8.extract = extractvalue [10 x [10 x i32]] %1, 9, 8
  %.fca.9.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 8
  store i32 %.fca.9.8.extract, ptr %.fca.9.8.gep, align 4
  %.fca.9.9.extract = extractvalue [10 x [10 x i32]] %1, 9, 9
  %.fca.9.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 9
  store i32 %.fca.9.9.extract, ptr %.fca.9.9.gep, align 4
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.9, ptr %0)
  %putchar.i = tail call i32 @putchar(i32 10)
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  %11 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  %12 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  %putchar2.i = tail call i32 @putchar(i32 10)
  %13 = load i32, ptr %matrix, align 4
  %14 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %13)
  %15 = load i32, ptr %.fca.0.1.gep, align 4
  %16 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %15)
  %17 = load i32, ptr %.fca.0.2.gep, align 4
  %18 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %17)
  %19 = load i32, ptr %.fca.0.3.gep, align 4
  %20 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %19)
  %21 = load i32, ptr %.fca.0.4.gep, align 4
  %22 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %21)
  %23 = load i32, ptr %.fca.0.5.gep, align 4
  %24 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %23)
  %25 = load i32, ptr %.fca.0.6.gep, align 4
  %26 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %25)
  %27 = load i32, ptr %.fca.0.7.gep, align 4
  %28 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %27)
  %29 = load i32, ptr %.fca.0.8.gep, align 4
  %30 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %29)
  %31 = load i32, ptr %.fca.0.9.gep, align 4
  %32 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %31)
  br label %if.exit.L34

if.exit.L34:                                      ; preds = %for.body.L33.peel.next, %if.exit.L34
  %indvars.iv = phi i64 [ 1, %for.body.L33.peel.next ], [ %indvars.iv.next, %if.exit.L34 ]
  %putchar.i6 = tail call i32 @putchar(i32 10)
  %33 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4)
  %34 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4)
  %35 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4)
  %36 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4)
  %37 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4)
  %38 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4)
  %39 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4)
  %40 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4)
  %41 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4)
  %42 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4)
  %putchar2.i7 = tail call i32 @putchar(i32 10)
  %43 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %indvars.iv, i64 0
  %44 = load i32, ptr %43, align 4
  %45 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %44)
  %46 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %indvars.iv, i64 1
  %47 = load i32, ptr %46, align 4
  %48 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %47)
  %49 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %indvars.iv, i64 2
  %50 = load i32, ptr %49, align 4
  %51 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %50)
  %52 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %indvars.iv, i64 3
  %53 = load i32, ptr %52, align 4
  %54 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %53)
  %55 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %indvars.iv, i64 4
  %56 = load i32, ptr %55, align 4
  %57 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %56)
  %58 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %indvars.iv, i64 5
  %59 = load i32, ptr %58, align 4
  %60 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %59)
  %61 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %indvars.iv, i64 6
  %62 = load i32, ptr %61, align 4
  %63 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %62)
  %64 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %indvars.iv, i64 7
  %65 = load i32, ptr %64, align 4
  %66 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %65)
  %67 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %indvars.iv, i64 8
  %68 = load i32, ptr %67, align 4
  %69 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %68)
  %70 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %indvars.iv, i64 9
  %71 = load i32, ptr %70, align 4
  %72 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i32 %71)
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 10
  br i1 %exitcond.not, label %for.exit.L33, label %if.exit.L34, !llvm.loop !0

for.exit.L33:                                     ; preds = %if.exit.L34
  %putchar.i8 = tail call i32 @putchar(i32 10)
  %73 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7)
  %74 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7)
  %75 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7)
  %76 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7)
  %77 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7)
  %78 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7)
  %79 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7)
  %80 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7)
  %81 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7)
  %82 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7)
  %putchar2.i9 = tail call i32 @putchar(i32 10)
  ret void
}

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define private fastcc i32 @_f__void__int__countLiveNeighborCell__intarrayarray_int_int([10 x [10 x i32]] %0, i32 %1, i32 %2) unnamed_addr #2 {
  %matrix = alloca [10 x [10 x i32]], align 4
  %.fca.0.0.extract = extractvalue [10 x [10 x i32]] %0, 0, 0
  store i32 %.fca.0.0.extract, ptr %matrix, align 4
  %.fca.0.1.extract = extractvalue [10 x [10 x i32]] %0, 0, 1
  %.fca.0.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 1
  store i32 %.fca.0.1.extract, ptr %.fca.0.1.gep, align 4
  %.fca.0.2.extract = extractvalue [10 x [10 x i32]] %0, 0, 2
  %.fca.0.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 2
  store i32 %.fca.0.2.extract, ptr %.fca.0.2.gep, align 4
  %.fca.0.3.extract = extractvalue [10 x [10 x i32]] %0, 0, 3
  %.fca.0.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 3
  store i32 %.fca.0.3.extract, ptr %.fca.0.3.gep, align 4
  %.fca.0.4.extract = extractvalue [10 x [10 x i32]] %0, 0, 4
  %.fca.0.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 4
  store i32 %.fca.0.4.extract, ptr %.fca.0.4.gep, align 4
  %.fca.0.5.extract = extractvalue [10 x [10 x i32]] %0, 0, 5
  %.fca.0.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 5
  store i32 %.fca.0.5.extract, ptr %.fca.0.5.gep, align 4
  %.fca.0.6.extract = extractvalue [10 x [10 x i32]] %0, 0, 6
  %.fca.0.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 6
  store i32 %.fca.0.6.extract, ptr %.fca.0.6.gep, align 4
  %.fca.0.7.extract = extractvalue [10 x [10 x i32]] %0, 0, 7
  %.fca.0.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 7
  store i32 %.fca.0.7.extract, ptr %.fca.0.7.gep, align 4
  %.fca.0.8.extract = extractvalue [10 x [10 x i32]] %0, 0, 8
  %.fca.0.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 8
  store i32 %.fca.0.8.extract, ptr %.fca.0.8.gep, align 4
  %.fca.0.9.extract = extractvalue [10 x [10 x i32]] %0, 0, 9
  %.fca.0.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 0, i64 9
  store i32 %.fca.0.9.extract, ptr %.fca.0.9.gep, align 4
  %.fca.1.0.extract = extractvalue [10 x [10 x i32]] %0, 1, 0
  %.fca.1.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 0
  store i32 %.fca.1.0.extract, ptr %.fca.1.0.gep, align 4
  %.fca.1.1.extract = extractvalue [10 x [10 x i32]] %0, 1, 1
  %.fca.1.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 1
  store i32 %.fca.1.1.extract, ptr %.fca.1.1.gep, align 4
  %.fca.1.2.extract = extractvalue [10 x [10 x i32]] %0, 1, 2
  %.fca.1.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 2
  store i32 %.fca.1.2.extract, ptr %.fca.1.2.gep, align 4
  %.fca.1.3.extract = extractvalue [10 x [10 x i32]] %0, 1, 3
  %.fca.1.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 3
  store i32 %.fca.1.3.extract, ptr %.fca.1.3.gep, align 4
  %.fca.1.4.extract = extractvalue [10 x [10 x i32]] %0, 1, 4
  %.fca.1.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 4
  store i32 %.fca.1.4.extract, ptr %.fca.1.4.gep, align 4
  %.fca.1.5.extract = extractvalue [10 x [10 x i32]] %0, 1, 5
  %.fca.1.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 5
  store i32 %.fca.1.5.extract, ptr %.fca.1.5.gep, align 4
  %.fca.1.6.extract = extractvalue [10 x [10 x i32]] %0, 1, 6
  %.fca.1.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 6
  store i32 %.fca.1.6.extract, ptr %.fca.1.6.gep, align 4
  %.fca.1.7.extract = extractvalue [10 x [10 x i32]] %0, 1, 7
  %.fca.1.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 7
  store i32 %.fca.1.7.extract, ptr %.fca.1.7.gep, align 4
  %.fca.1.8.extract = extractvalue [10 x [10 x i32]] %0, 1, 8
  %.fca.1.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 8
  store i32 %.fca.1.8.extract, ptr %.fca.1.8.gep, align 4
  %.fca.1.9.extract = extractvalue [10 x [10 x i32]] %0, 1, 9
  %.fca.1.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 1, i64 9
  store i32 %.fca.1.9.extract, ptr %.fca.1.9.gep, align 4
  %.fca.2.0.extract = extractvalue [10 x [10 x i32]] %0, 2, 0
  %.fca.2.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 0
  store i32 %.fca.2.0.extract, ptr %.fca.2.0.gep, align 4
  %.fca.2.1.extract = extractvalue [10 x [10 x i32]] %0, 2, 1
  %.fca.2.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 1
  store i32 %.fca.2.1.extract, ptr %.fca.2.1.gep, align 4
  %.fca.2.2.extract = extractvalue [10 x [10 x i32]] %0, 2, 2
  %.fca.2.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 2
  store i32 %.fca.2.2.extract, ptr %.fca.2.2.gep, align 4
  %.fca.2.3.extract = extractvalue [10 x [10 x i32]] %0, 2, 3
  %.fca.2.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 3
  store i32 %.fca.2.3.extract, ptr %.fca.2.3.gep, align 4
  %.fca.2.4.extract = extractvalue [10 x [10 x i32]] %0, 2, 4
  %.fca.2.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 4
  store i32 %.fca.2.4.extract, ptr %.fca.2.4.gep, align 4
  %.fca.2.5.extract = extractvalue [10 x [10 x i32]] %0, 2, 5
  %.fca.2.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 5
  store i32 %.fca.2.5.extract, ptr %.fca.2.5.gep, align 4
  %.fca.2.6.extract = extractvalue [10 x [10 x i32]] %0, 2, 6
  %.fca.2.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 6
  store i32 %.fca.2.6.extract, ptr %.fca.2.6.gep, align 4
  %.fca.2.7.extract = extractvalue [10 x [10 x i32]] %0, 2, 7
  %.fca.2.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 7
  store i32 %.fca.2.7.extract, ptr %.fca.2.7.gep, align 4
  %.fca.2.8.extract = extractvalue [10 x [10 x i32]] %0, 2, 8
  %.fca.2.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 8
  store i32 %.fca.2.8.extract, ptr %.fca.2.8.gep, align 4
  %.fca.2.9.extract = extractvalue [10 x [10 x i32]] %0, 2, 9
  %.fca.2.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 2, i64 9
  store i32 %.fca.2.9.extract, ptr %.fca.2.9.gep, align 4
  %.fca.3.0.extract = extractvalue [10 x [10 x i32]] %0, 3, 0
  %.fca.3.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 0
  store i32 %.fca.3.0.extract, ptr %.fca.3.0.gep, align 4
  %.fca.3.1.extract = extractvalue [10 x [10 x i32]] %0, 3, 1
  %.fca.3.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 1
  store i32 %.fca.3.1.extract, ptr %.fca.3.1.gep, align 4
  %.fca.3.2.extract = extractvalue [10 x [10 x i32]] %0, 3, 2
  %.fca.3.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 2
  store i32 %.fca.3.2.extract, ptr %.fca.3.2.gep, align 4
  %.fca.3.3.extract = extractvalue [10 x [10 x i32]] %0, 3, 3
  %.fca.3.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 3
  store i32 %.fca.3.3.extract, ptr %.fca.3.3.gep, align 4
  %.fca.3.4.extract = extractvalue [10 x [10 x i32]] %0, 3, 4
  %.fca.3.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 4
  store i32 %.fca.3.4.extract, ptr %.fca.3.4.gep, align 4
  %.fca.3.5.extract = extractvalue [10 x [10 x i32]] %0, 3, 5
  %.fca.3.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 5
  store i32 %.fca.3.5.extract, ptr %.fca.3.5.gep, align 4
  %.fca.3.6.extract = extractvalue [10 x [10 x i32]] %0, 3, 6
  %.fca.3.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 6
  store i32 %.fca.3.6.extract, ptr %.fca.3.6.gep, align 4
  %.fca.3.7.extract = extractvalue [10 x [10 x i32]] %0, 3, 7
  %.fca.3.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 7
  store i32 %.fca.3.7.extract, ptr %.fca.3.7.gep, align 4
  %.fca.3.8.extract = extractvalue [10 x [10 x i32]] %0, 3, 8
  %.fca.3.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 8
  store i32 %.fca.3.8.extract, ptr %.fca.3.8.gep, align 4
  %.fca.3.9.extract = extractvalue [10 x [10 x i32]] %0, 3, 9
  %.fca.3.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 3, i64 9
  store i32 %.fca.3.9.extract, ptr %.fca.3.9.gep, align 4
  %.fca.4.0.extract = extractvalue [10 x [10 x i32]] %0, 4, 0
  %.fca.4.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 0
  store i32 %.fca.4.0.extract, ptr %.fca.4.0.gep, align 4
  %.fca.4.1.extract = extractvalue [10 x [10 x i32]] %0, 4, 1
  %.fca.4.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 1
  store i32 %.fca.4.1.extract, ptr %.fca.4.1.gep, align 4
  %.fca.4.2.extract = extractvalue [10 x [10 x i32]] %0, 4, 2
  %.fca.4.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 2
  store i32 %.fca.4.2.extract, ptr %.fca.4.2.gep, align 4
  %.fca.4.3.extract = extractvalue [10 x [10 x i32]] %0, 4, 3
  %.fca.4.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 3
  store i32 %.fca.4.3.extract, ptr %.fca.4.3.gep, align 4
  %.fca.4.4.extract = extractvalue [10 x [10 x i32]] %0, 4, 4
  %.fca.4.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 4
  store i32 %.fca.4.4.extract, ptr %.fca.4.4.gep, align 4
  %.fca.4.5.extract = extractvalue [10 x [10 x i32]] %0, 4, 5
  %.fca.4.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 5
  store i32 %.fca.4.5.extract, ptr %.fca.4.5.gep, align 4
  %.fca.4.6.extract = extractvalue [10 x [10 x i32]] %0, 4, 6
  %.fca.4.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 6
  store i32 %.fca.4.6.extract, ptr %.fca.4.6.gep, align 4
  %.fca.4.7.extract = extractvalue [10 x [10 x i32]] %0, 4, 7
  %.fca.4.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 7
  store i32 %.fca.4.7.extract, ptr %.fca.4.7.gep, align 4
  %.fca.4.8.extract = extractvalue [10 x [10 x i32]] %0, 4, 8
  %.fca.4.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 8
  store i32 %.fca.4.8.extract, ptr %.fca.4.8.gep, align 4
  %.fca.4.9.extract = extractvalue [10 x [10 x i32]] %0, 4, 9
  %.fca.4.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 4, i64 9
  store i32 %.fca.4.9.extract, ptr %.fca.4.9.gep, align 4
  %.fca.5.0.extract = extractvalue [10 x [10 x i32]] %0, 5, 0
  %.fca.5.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 0
  store i32 %.fca.5.0.extract, ptr %.fca.5.0.gep, align 4
  %.fca.5.1.extract = extractvalue [10 x [10 x i32]] %0, 5, 1
  %.fca.5.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 1
  store i32 %.fca.5.1.extract, ptr %.fca.5.1.gep, align 4
  %.fca.5.2.extract = extractvalue [10 x [10 x i32]] %0, 5, 2
  %.fca.5.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 2
  store i32 %.fca.5.2.extract, ptr %.fca.5.2.gep, align 4
  %.fca.5.3.extract = extractvalue [10 x [10 x i32]] %0, 5, 3
  %.fca.5.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 3
  store i32 %.fca.5.3.extract, ptr %.fca.5.3.gep, align 4
  %.fca.5.4.extract = extractvalue [10 x [10 x i32]] %0, 5, 4
  %.fca.5.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 4
  store i32 %.fca.5.4.extract, ptr %.fca.5.4.gep, align 4
  %.fca.5.5.extract = extractvalue [10 x [10 x i32]] %0, 5, 5
  %.fca.5.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 5
  store i32 %.fca.5.5.extract, ptr %.fca.5.5.gep, align 4
  %.fca.5.6.extract = extractvalue [10 x [10 x i32]] %0, 5, 6
  %.fca.5.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 6
  store i32 %.fca.5.6.extract, ptr %.fca.5.6.gep, align 4
  %.fca.5.7.extract = extractvalue [10 x [10 x i32]] %0, 5, 7
  %.fca.5.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 7
  store i32 %.fca.5.7.extract, ptr %.fca.5.7.gep, align 4
  %.fca.5.8.extract = extractvalue [10 x [10 x i32]] %0, 5, 8
  %.fca.5.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 8
  store i32 %.fca.5.8.extract, ptr %.fca.5.8.gep, align 4
  %.fca.5.9.extract = extractvalue [10 x [10 x i32]] %0, 5, 9
  %.fca.5.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 5, i64 9
  store i32 %.fca.5.9.extract, ptr %.fca.5.9.gep, align 4
  %.fca.6.0.extract = extractvalue [10 x [10 x i32]] %0, 6, 0
  %.fca.6.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 0
  store i32 %.fca.6.0.extract, ptr %.fca.6.0.gep, align 4
  %.fca.6.1.extract = extractvalue [10 x [10 x i32]] %0, 6, 1
  %.fca.6.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 1
  store i32 %.fca.6.1.extract, ptr %.fca.6.1.gep, align 4
  %.fca.6.2.extract = extractvalue [10 x [10 x i32]] %0, 6, 2
  %.fca.6.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 2
  store i32 %.fca.6.2.extract, ptr %.fca.6.2.gep, align 4
  %.fca.6.3.extract = extractvalue [10 x [10 x i32]] %0, 6, 3
  %.fca.6.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 3
  store i32 %.fca.6.3.extract, ptr %.fca.6.3.gep, align 4
  %.fca.6.4.extract = extractvalue [10 x [10 x i32]] %0, 6, 4
  %.fca.6.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 4
  store i32 %.fca.6.4.extract, ptr %.fca.6.4.gep, align 4
  %.fca.6.5.extract = extractvalue [10 x [10 x i32]] %0, 6, 5
  %.fca.6.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 5
  store i32 %.fca.6.5.extract, ptr %.fca.6.5.gep, align 4
  %.fca.6.6.extract = extractvalue [10 x [10 x i32]] %0, 6, 6
  %.fca.6.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 6
  store i32 %.fca.6.6.extract, ptr %.fca.6.6.gep, align 4
  %.fca.6.7.extract = extractvalue [10 x [10 x i32]] %0, 6, 7
  %.fca.6.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 7
  store i32 %.fca.6.7.extract, ptr %.fca.6.7.gep, align 4
  %.fca.6.8.extract = extractvalue [10 x [10 x i32]] %0, 6, 8
  %.fca.6.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 8
  store i32 %.fca.6.8.extract, ptr %.fca.6.8.gep, align 4
  %.fca.6.9.extract = extractvalue [10 x [10 x i32]] %0, 6, 9
  %.fca.6.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 6, i64 9
  store i32 %.fca.6.9.extract, ptr %.fca.6.9.gep, align 4
  %.fca.7.0.extract = extractvalue [10 x [10 x i32]] %0, 7, 0
  %.fca.7.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 0
  store i32 %.fca.7.0.extract, ptr %.fca.7.0.gep, align 4
  %.fca.7.1.extract = extractvalue [10 x [10 x i32]] %0, 7, 1
  %.fca.7.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 1
  store i32 %.fca.7.1.extract, ptr %.fca.7.1.gep, align 4
  %.fca.7.2.extract = extractvalue [10 x [10 x i32]] %0, 7, 2
  %.fca.7.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 2
  store i32 %.fca.7.2.extract, ptr %.fca.7.2.gep, align 4
  %.fca.7.3.extract = extractvalue [10 x [10 x i32]] %0, 7, 3
  %.fca.7.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 3
  store i32 %.fca.7.3.extract, ptr %.fca.7.3.gep, align 4
  %.fca.7.4.extract = extractvalue [10 x [10 x i32]] %0, 7, 4
  %.fca.7.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 4
  store i32 %.fca.7.4.extract, ptr %.fca.7.4.gep, align 4
  %.fca.7.5.extract = extractvalue [10 x [10 x i32]] %0, 7, 5
  %.fca.7.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 5
  store i32 %.fca.7.5.extract, ptr %.fca.7.5.gep, align 4
  %.fca.7.6.extract = extractvalue [10 x [10 x i32]] %0, 7, 6
  %.fca.7.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 6
  store i32 %.fca.7.6.extract, ptr %.fca.7.6.gep, align 4
  %.fca.7.7.extract = extractvalue [10 x [10 x i32]] %0, 7, 7
  %.fca.7.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 7
  store i32 %.fca.7.7.extract, ptr %.fca.7.7.gep, align 4
  %.fca.7.8.extract = extractvalue [10 x [10 x i32]] %0, 7, 8
  %.fca.7.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 8
  store i32 %.fca.7.8.extract, ptr %.fca.7.8.gep, align 4
  %.fca.7.9.extract = extractvalue [10 x [10 x i32]] %0, 7, 9
  %.fca.7.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 7, i64 9
  store i32 %.fca.7.9.extract, ptr %.fca.7.9.gep, align 4
  %.fca.8.0.extract = extractvalue [10 x [10 x i32]] %0, 8, 0
  %.fca.8.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 0
  store i32 %.fca.8.0.extract, ptr %.fca.8.0.gep, align 4
  %.fca.8.1.extract = extractvalue [10 x [10 x i32]] %0, 8, 1
  %.fca.8.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 1
  store i32 %.fca.8.1.extract, ptr %.fca.8.1.gep, align 4
  %.fca.8.2.extract = extractvalue [10 x [10 x i32]] %0, 8, 2
  %.fca.8.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 2
  store i32 %.fca.8.2.extract, ptr %.fca.8.2.gep, align 4
  %.fca.8.3.extract = extractvalue [10 x [10 x i32]] %0, 8, 3
  %.fca.8.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 3
  store i32 %.fca.8.3.extract, ptr %.fca.8.3.gep, align 4
  %.fca.8.4.extract = extractvalue [10 x [10 x i32]] %0, 8, 4
  %.fca.8.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 4
  store i32 %.fca.8.4.extract, ptr %.fca.8.4.gep, align 4
  %.fca.8.5.extract = extractvalue [10 x [10 x i32]] %0, 8, 5
  %.fca.8.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 5
  store i32 %.fca.8.5.extract, ptr %.fca.8.5.gep, align 4
  %.fca.8.6.extract = extractvalue [10 x [10 x i32]] %0, 8, 6
  %.fca.8.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 6
  store i32 %.fca.8.6.extract, ptr %.fca.8.6.gep, align 4
  %.fca.8.7.extract = extractvalue [10 x [10 x i32]] %0, 8, 7
  %.fca.8.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 7
  store i32 %.fca.8.7.extract, ptr %.fca.8.7.gep, align 4
  %.fca.8.8.extract = extractvalue [10 x [10 x i32]] %0, 8, 8
  %.fca.8.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 8
  store i32 %.fca.8.8.extract, ptr %.fca.8.8.gep, align 4
  %.fca.8.9.extract = extractvalue [10 x [10 x i32]] %0, 8, 9
  %.fca.8.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 8, i64 9
  store i32 %.fca.8.9.extract, ptr %.fca.8.9.gep, align 4
  %.fca.9.0.extract = extractvalue [10 x [10 x i32]] %0, 9, 0
  %.fca.9.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 0
  store i32 %.fca.9.0.extract, ptr %.fca.9.0.gep, align 4
  %.fca.9.1.extract = extractvalue [10 x [10 x i32]] %0, 9, 1
  %.fca.9.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 1
  store i32 %.fca.9.1.extract, ptr %.fca.9.1.gep, align 4
  %.fca.9.2.extract = extractvalue [10 x [10 x i32]] %0, 9, 2
  %.fca.9.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 2
  store i32 %.fca.9.2.extract, ptr %.fca.9.2.gep, align 4
  %.fca.9.3.extract = extractvalue [10 x [10 x i32]] %0, 9, 3
  %.fca.9.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 3
  store i32 %.fca.9.3.extract, ptr %.fca.9.3.gep, align 4
  %.fca.9.4.extract = extractvalue [10 x [10 x i32]] %0, 9, 4
  %.fca.9.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 4
  store i32 %.fca.9.4.extract, ptr %.fca.9.4.gep, align 4
  %.fca.9.5.extract = extractvalue [10 x [10 x i32]] %0, 9, 5
  %.fca.9.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 5
  store i32 %.fca.9.5.extract, ptr %.fca.9.5.gep, align 4
  %.fca.9.6.extract = extractvalue [10 x [10 x i32]] %0, 9, 6
  %.fca.9.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 6
  store i32 %.fca.9.6.extract, ptr %.fca.9.6.gep, align 4
  %.fca.9.7.extract = extractvalue [10 x [10 x i32]] %0, 9, 7
  %.fca.9.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 7
  store i32 %.fca.9.7.extract, ptr %.fca.9.7.gep, align 4
  %.fca.9.8.extract = extractvalue [10 x [10 x i32]] %0, 9, 8
  %.fca.9.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 8
  store i32 %.fca.9.8.extract, ptr %.fca.9.8.gep, align 4
  %.fca.9.9.extract = extractvalue [10 x [10 x i32]] %0, 9, 9
  %.fca.9.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 9, i64 9
  store i32 %.fca.9.9.extract, ptr %.fca.9.9.gep, align 4
  %4 = add i32 %1, -1
  %5 = add i32 %1, 1
  %.not22 = icmp sgt i32 %4, %5
  br i1 %.not22, label %for.exit.L46, label %for.body.L46.lr.ph

for.body.L46.lr.ph:                               ; preds = %3
  %6 = add i32 %2, -1
  %7 = add i32 %2, 1
  %.not1819 = icmp sgt i32 %6, %7
  br label %for.body.L46

for.body.L46:                                     ; preds = %for.body.L46.lr.ph, %for.tail.L46
  %i.024 = phi i32 [ %4, %for.body.L46.lr.ph ], [ %23, %for.tail.L46 ]
  %count.023 = phi i32 [ 0, %for.body.L46.lr.ph ], [ %count.1.lcssa, %for.tail.L46 ]
  br i1 %.not1819, label %for.tail.L46, label %for.body.L47.lr.ph

for.body.L47.lr.ph:                               ; preds = %for.body.L46
  %8 = icmp eq i32 %i.024, %1
  %9 = icmp slt i32 %i.024, 0
  %10 = icmp ugt i32 %i.024, 9
  %11 = zext i32 %i.024 to i64
  br label %for.body.L47

for.body.L47:                                     ; preds = %for.body.L47.lr.ph, %for.tail.L47
  %j.021 = phi i32 [ %6, %for.body.L47.lr.ph ], [ %22, %for.tail.L47 ]
  %count.120 = phi i32 [ %count.023, %for.body.L47.lr.ph ], [ %count.2, %for.tail.L47 ]
  %12 = icmp eq i32 %j.021, %2
  %land_phi = select i1 %8, i1 %12, i1 false
  %13 = icmp slt i32 %j.021, 0
  %14 = or i1 %land_phi, %9
  %or.cond = select i1 %14, i1 true, i1 %13
  %15 = icmp ugt i32 %j.021, 9
  %16 = select i1 %or.cond, i1 true, i1 %10
  %or.cond26 = select i1 %16, i1 true, i1 %15
  br i1 %or.cond26, label %for.tail.L47, label %if.exit.L48

if.exit.L48:                                      ; preds = %for.body.L47
  %17 = zext i32 %j.021 to i64
  %18 = getelementptr inbounds [10 x [10 x i32]], ptr %matrix, i64 0, i64 %11, i64 %17
  %19 = load i32, ptr %18, align 4
  %20 = icmp eq i32 %19, 1
  %21 = zext i1 %20 to i32
  %spec.select = add i32 %count.120, %21
  br label %for.tail.L47

for.tail.L47:                                     ; preds = %if.exit.L48, %for.body.L47
  %count.2 = phi i32 [ %spec.select, %if.exit.L48 ], [ %count.120, %for.body.L47 ]
  %22 = add i32 %j.021, 1
  %.not18 = icmp sgt i32 %22, %7
  br i1 %.not18, label %for.tail.L46, label %for.body.L47

for.tail.L46:                                     ; preds = %for.tail.L47, %for.body.L46
  %count.1.lcssa = phi i32 [ %count.023, %for.body.L46 ], [ %count.2, %for.tail.L47 ]
  %23 = add i32 %i.024, 1
  %.not = icmp sgt i32 %23, %5
  br i1 %.not, label %for.exit.L46, label %for.body.L46

for.exit.L46:                                     ; preds = %for.tail.L46, %3
  %count.0.lcssa = phi i32 [ 0, %3 ], [ %count.1.lcssa, %for.tail.L46 ]
  ret i32 %count.0.lcssa
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #3 {
  %a = alloca [10 x [10 x i32]], align 4
  %b = alloca [10 x [10 x i32]], align 4
  %.fca.0.1.gep426 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 0, i64 1
  %.fca.0.2.gep427 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 0, i64 2
  %.fca.0.3.gep428 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 0, i64 3
  %.fca.0.4.gep429 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 0, i64 4
  %.fca.0.5.gep430 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 0, i64 5
  %.fca.0.6.gep431 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 0, i64 6
  %.fca.0.7.gep432 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 0, i64 7
  %.fca.0.8.gep433 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 0, i64 8
  %.fca.0.9.gep434 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 0, i64 9
  %.fca.1.0.gep435 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 1, i64 0
  %.fca.1.1.gep436 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 1, i64 1
  %.fca.1.2.gep437 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 1, i64 2
  %.fca.1.3.gep438 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 1, i64 3
  %.fca.1.4.gep439 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 1, i64 4
  %.fca.1.5.gep440 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 1, i64 5
  %.fca.1.6.gep441 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 1, i64 6
  %.fca.1.7.gep442 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 1, i64 7
  %.fca.1.8.gep443 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 1, i64 8
  %.fca.1.9.gep444 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 1, i64 9
  %.fca.2.0.gep445 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 2, i64 0
  %.fca.2.1.gep446 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 2, i64 1
  %.fca.2.2.gep447 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 2, i64 2
  %.fca.2.3.gep448 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 2, i64 3
  %.fca.2.4.gep449 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 2, i64 4
  %.fca.2.5.gep450 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 2, i64 5
  %.fca.2.6.gep451 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 2, i64 6
  %.fca.2.7.gep452 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 2, i64 7
  %.fca.2.8.gep453 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 2, i64 8
  %.fca.2.9.gep454 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 2, i64 9
  %.fca.3.0.gep455 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 3, i64 0
  %.fca.3.1.gep456 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 3, i64 1
  %.fca.3.2.gep457 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 3, i64 2
  %.fca.3.3.gep458 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 3, i64 3
  %.fca.3.4.gep459 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 3, i64 4
  %.fca.3.5.gep460 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 3, i64 5
  %.fca.3.6.gep461 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 3, i64 6
  %.fca.3.7.gep462 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 3, i64 7
  %.fca.3.8.gep463 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 3, i64 8
  %.fca.3.9.gep464 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 3, i64 9
  %.fca.4.0.gep465 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 4, i64 0
  %.fca.4.1.gep466 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 4, i64 1
  %.fca.4.2.gep467 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 4, i64 2
  %.fca.4.3.gep468 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 4, i64 3
  %.fca.4.4.gep469 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 4, i64 4
  %.fca.4.5.gep470 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 4, i64 5
  %.fca.4.6.gep471 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 4, i64 6
  %.fca.4.7.gep472 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 4, i64 7
  %.fca.4.8.gep473 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 4, i64 8
  %.fca.4.9.gep474 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 4, i64 9
  %.fca.5.0.gep475 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 5, i64 0
  %.fca.5.1.gep476 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 5, i64 1
  %.fca.5.2.gep477 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 5, i64 2
  %.fca.5.3.gep478 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 5, i64 3
  %.fca.5.4.gep479 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 5, i64 4
  %.fca.5.5.gep480 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 5, i64 5
  %.fca.5.6.gep481 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 5, i64 6
  %.fca.5.7.gep482 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 5, i64 7
  %.fca.5.8.gep483 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 5, i64 8
  %.fca.5.9.gep484 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 5, i64 9
  %.fca.6.0.gep485 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 6, i64 0
  %.fca.6.1.gep486 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 6, i64 1
  %.fca.6.2.gep487 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 6, i64 2
  %.fca.6.3.gep488 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 6, i64 3
  %.fca.6.4.gep489 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 6, i64 4
  %.fca.6.5.gep490 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 6, i64 5
  %.fca.6.6.gep491 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 6, i64 6
  %.fca.6.7.gep492 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 6, i64 7
  %.fca.6.8.gep493 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 6, i64 8
  %.fca.6.9.gep494 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 6, i64 9
  %.fca.7.0.gep495 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 7, i64 0
  %.fca.7.1.gep496 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 7, i64 1
  %.fca.7.2.gep497 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 7, i64 2
  %.fca.7.3.gep498 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 7, i64 3
  %.fca.7.4.gep499 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 7, i64 4
  %.fca.7.5.gep500 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 7, i64 5
  %.fca.7.6.gep501 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 7, i64 6
  %.fca.7.7.gep502 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 7, i64 7
  %.fca.7.8.gep503 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 7, i64 8
  %.fca.7.9.gep504 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 7, i64 9
  %.fca.8.0.gep505 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 8, i64 0
  %.fca.8.1.gep506 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 8, i64 1
  %.fca.8.2.gep507 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 8, i64 2
  %.fca.8.3.gep508 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 8, i64 3
  %.fca.8.4.gep509 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 8, i64 4
  %.fca.8.5.gep510 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 8, i64 5
  %.fca.8.6.gep511 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 8, i64 6
  %.fca.8.7.gep512 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 8, i64 7
  %.fca.8.8.gep513 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 8, i64 8
  %.fca.8.9.gep514 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 8, i64 9
  %.fca.9.0.gep515 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 9, i64 0
  %.fca.9.1.gep516 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 9, i64 1
  %.fca.9.2.gep517 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 9, i64 2
  %.fca.9.3.gep518 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 9, i64 3
  %.fca.9.4.gep519 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 9, i64 4
  %.fca.9.5.gep520 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 9, i64 5
  %.fca.9.6.gep521 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 9, i64 6
  %.fca.9.7.gep522 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 9, i64 7
  %.fca.9.8.gep523 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 9, i64 8
  %.fca.9.9.gep524 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 9, i64 9
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(400) %a, i8 0, i64 400, i1 false)
  %.fca.0.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 0, i64 1
  %.fca.0.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 0, i64 2
  %.fca.0.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 0, i64 3
  %.fca.0.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 0, i64 4
  %.fca.0.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 0, i64 5
  %.fca.0.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 0, i64 6
  %.fca.0.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 0, i64 7
  %.fca.0.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 0, i64 8
  %.fca.0.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 0, i64 9
  %.fca.1.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 1, i64 0
  %.fca.1.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 1, i64 1
  %.fca.1.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 1, i64 2
  %.fca.1.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 1, i64 3
  %.fca.1.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 1, i64 4
  %.fca.1.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 1, i64 5
  %.fca.1.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 1, i64 6
  %.fca.1.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 1, i64 7
  %.fca.1.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 1, i64 8
  %.fca.1.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 1, i64 9
  %.fca.2.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 2, i64 0
  %.fca.2.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 2, i64 1
  %.fca.2.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 2, i64 2
  %.fca.2.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 2, i64 3
  %.fca.2.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 2, i64 4
  %.fca.2.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 2, i64 5
  %.fca.2.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 2, i64 6
  %.fca.2.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 2, i64 7
  %.fca.2.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 2, i64 8
  %.fca.2.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 2, i64 9
  %.fca.3.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 3, i64 0
  %.fca.3.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 3, i64 1
  %.fca.3.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 3, i64 2
  %.fca.3.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 3, i64 3
  %.fca.3.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 3, i64 4
  %.fca.3.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 3, i64 5
  %.fca.3.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 3, i64 6
  %.fca.3.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 3, i64 7
  %.fca.3.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 3, i64 8
  %.fca.3.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 3, i64 9
  %.fca.4.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 4, i64 0
  %.fca.4.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 4, i64 1
  %.fca.4.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 4, i64 2
  %.fca.4.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 4, i64 3
  %.fca.4.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 4, i64 4
  %.fca.4.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 4, i64 5
  %.fca.4.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 4, i64 6
  %.fca.4.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 4, i64 7
  %.fca.4.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 4, i64 8
  %.fca.4.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 4, i64 9
  %.fca.5.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 5, i64 0
  %.fca.5.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 5, i64 1
  %.fca.5.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 5, i64 2
  %.fca.5.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 5, i64 3
  %.fca.5.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 5, i64 4
  %.fca.5.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 5, i64 5
  %.fca.5.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 5, i64 6
  %.fca.5.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 5, i64 7
  %.fca.5.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 5, i64 8
  %.fca.5.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 5, i64 9
  %.fca.6.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 6, i64 0
  %.fca.6.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 6, i64 1
  %.fca.6.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 6, i64 2
  %.fca.6.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 6, i64 3
  %.fca.6.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 6, i64 4
  %.fca.6.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 6, i64 5
  %.fca.6.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 6, i64 6
  %.fca.6.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 6, i64 7
  %.fca.6.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 6, i64 8
  %.fca.6.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 6, i64 9
  %.fca.7.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 7, i64 0
  %.fca.7.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 7, i64 1
  %.fca.7.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 7, i64 2
  %.fca.7.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 7, i64 3
  %.fca.7.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 7, i64 4
  %.fca.7.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 7, i64 5
  %.fca.7.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 7, i64 6
  %.fca.7.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 7, i64 7
  %.fca.7.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 7, i64 8
  %.fca.7.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 7, i64 9
  %.fca.8.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 8, i64 0
  %.fca.8.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 8, i64 1
  %.fca.8.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 8, i64 2
  %.fca.8.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 8, i64 3
  %.fca.8.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 8, i64 4
  %.fca.8.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 8, i64 5
  %.fca.8.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 8, i64 6
  %.fca.8.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 8, i64 7
  %.fca.8.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 8, i64 8
  %.fca.8.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 8, i64 9
  %.fca.9.0.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 9, i64 0
  %.fca.9.1.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 9, i64 1
  %.fca.9.2.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 9, i64 2
  %.fca.9.3.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 9, i64 3
  %.fca.9.4.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 9, i64 4
  %.fca.9.5.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 9, i64 5
  %.fca.9.6.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 9, i64 6
  %.fca.9.7.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 9, i64 7
  %.fca.9.8.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 9, i64 8
  %.fca.9.9.gep = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 9, i64 9
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(400) %b, i8 0, i64 400, i1 false)
  br label %for.head.L65.preheader

for.head.L65.preheader:                           ; preds = %0, %for.head.L65.preheader
  %indvars.iv = phi i64 [ 0, %0 ], [ %indvars.iv.next, %for.head.L65.preheader ]
  %1 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv, i64 0
  %2 = tail call fastcc i32 @_f__void__int__genFakeRandInt() #5
  store i32 %2, ptr %1, align 4
  %3 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv, i64 1
  %4 = tail call fastcc i32 @_f__void__int__genFakeRandInt() #5
  store i32 %4, ptr %3, align 4
  %5 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv, i64 2
  %6 = tail call fastcc i32 @_f__void__int__genFakeRandInt() #5
  store i32 %6, ptr %5, align 4
  %7 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv, i64 3
  %8 = tail call fastcc i32 @_f__void__int__genFakeRandInt() #5
  store i32 %8, ptr %7, align 4
  %9 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv, i64 4
  %10 = tail call fastcc i32 @_f__void__int__genFakeRandInt() #5
  store i32 %10, ptr %9, align 4
  %11 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv, i64 5
  %12 = tail call fastcc i32 @_f__void__int__genFakeRandInt() #5
  store i32 %12, ptr %11, align 4
  %13 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv, i64 6
  %14 = tail call fastcc i32 @_f__void__int__genFakeRandInt() #5
  store i32 %14, ptr %13, align 4
  %15 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv, i64 7
  %16 = tail call fastcc i32 @_f__void__int__genFakeRandInt() #5
  store i32 %16, ptr %15, align 4
  %17 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv, i64 8
  %18 = tail call fastcc i32 @_f__void__int__genFakeRandInt() #5
  store i32 %18, ptr %17, align 4
  %19 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv, i64 9
  %20 = tail call fastcc i32 @_f__void__int__genFakeRandInt() #5
  store i32 %20, ptr %19, align 4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 10
  br i1 %exitcond.not, label %for.exit.L64, label %for.head.L65.preheader

for.exit.L64:                                     ; preds = %for.head.L65.preheader
  %.fca.0.0.load526 = load i32, ptr %a, align 4
  %.fca.0.0.insert527 = insertvalue [10 x [10 x i32]] poison, i32 %.fca.0.0.load526, 0, 0
  %.fca.0.1.load529 = load i32, ptr %.fca.0.1.gep426, align 4
  %.fca.0.1.insert530 = insertvalue [10 x [10 x i32]] %.fca.0.0.insert527, i32 %.fca.0.1.load529, 0, 1
  %.fca.0.2.load532 = load i32, ptr %.fca.0.2.gep427, align 4
  %.fca.0.2.insert533 = insertvalue [10 x [10 x i32]] %.fca.0.1.insert530, i32 %.fca.0.2.load532, 0, 2
  %.fca.0.3.load535 = load i32, ptr %.fca.0.3.gep428, align 4
  %.fca.0.3.insert536 = insertvalue [10 x [10 x i32]] %.fca.0.2.insert533, i32 %.fca.0.3.load535, 0, 3
  %.fca.0.4.load538 = load i32, ptr %.fca.0.4.gep429, align 4
  %.fca.0.4.insert539 = insertvalue [10 x [10 x i32]] %.fca.0.3.insert536, i32 %.fca.0.4.load538, 0, 4
  %.fca.0.5.load541 = load i32, ptr %.fca.0.5.gep430, align 4
  %.fca.0.5.insert542 = insertvalue [10 x [10 x i32]] %.fca.0.4.insert539, i32 %.fca.0.5.load541, 0, 5
  %.fca.0.6.load544 = load i32, ptr %.fca.0.6.gep431, align 4
  %.fca.0.6.insert545 = insertvalue [10 x [10 x i32]] %.fca.0.5.insert542, i32 %.fca.0.6.load544, 0, 6
  %.fca.0.7.load547 = load i32, ptr %.fca.0.7.gep432, align 4
  %.fca.0.7.insert548 = insertvalue [10 x [10 x i32]] %.fca.0.6.insert545, i32 %.fca.0.7.load547, 0, 7
  %.fca.0.8.load550 = load i32, ptr %.fca.0.8.gep433, align 4
  %.fca.0.8.insert551 = insertvalue [10 x [10 x i32]] %.fca.0.7.insert548, i32 %.fca.0.8.load550, 0, 8
  %.fca.0.9.load553 = load i32, ptr %.fca.0.9.gep434, align 4
  %.fca.0.9.insert554 = insertvalue [10 x [10 x i32]] %.fca.0.8.insert551, i32 %.fca.0.9.load553, 0, 9
  %.fca.1.0.load556 = load i32, ptr %.fca.1.0.gep435, align 4
  %.fca.1.0.insert557 = insertvalue [10 x [10 x i32]] %.fca.0.9.insert554, i32 %.fca.1.0.load556, 1, 0
  %.fca.1.1.load559 = load i32, ptr %.fca.1.1.gep436, align 4
  %.fca.1.1.insert560 = insertvalue [10 x [10 x i32]] %.fca.1.0.insert557, i32 %.fca.1.1.load559, 1, 1
  %.fca.1.2.load562 = load i32, ptr %.fca.1.2.gep437, align 4
  %.fca.1.2.insert563 = insertvalue [10 x [10 x i32]] %.fca.1.1.insert560, i32 %.fca.1.2.load562, 1, 2
  %.fca.1.3.load565 = load i32, ptr %.fca.1.3.gep438, align 4
  %.fca.1.3.insert566 = insertvalue [10 x [10 x i32]] %.fca.1.2.insert563, i32 %.fca.1.3.load565, 1, 3
  %.fca.1.4.load568 = load i32, ptr %.fca.1.4.gep439, align 4
  %.fca.1.4.insert569 = insertvalue [10 x [10 x i32]] %.fca.1.3.insert566, i32 %.fca.1.4.load568, 1, 4
  %.fca.1.5.load571 = load i32, ptr %.fca.1.5.gep440, align 4
  %.fca.1.5.insert572 = insertvalue [10 x [10 x i32]] %.fca.1.4.insert569, i32 %.fca.1.5.load571, 1, 5
  %.fca.1.6.load574 = load i32, ptr %.fca.1.6.gep441, align 4
  %.fca.1.6.insert575 = insertvalue [10 x [10 x i32]] %.fca.1.5.insert572, i32 %.fca.1.6.load574, 1, 6
  %.fca.1.7.load577 = load i32, ptr %.fca.1.7.gep442, align 4
  %.fca.1.7.insert578 = insertvalue [10 x [10 x i32]] %.fca.1.6.insert575, i32 %.fca.1.7.load577, 1, 7
  %.fca.1.8.load580 = load i32, ptr %.fca.1.8.gep443, align 4
  %.fca.1.8.insert581 = insertvalue [10 x [10 x i32]] %.fca.1.7.insert578, i32 %.fca.1.8.load580, 1, 8
  %.fca.1.9.load583 = load i32, ptr %.fca.1.9.gep444, align 4
  %.fca.1.9.insert584 = insertvalue [10 x [10 x i32]] %.fca.1.8.insert581, i32 %.fca.1.9.load583, 1, 9
  %.fca.2.0.load586 = load i32, ptr %.fca.2.0.gep445, align 4
  %.fca.2.0.insert587 = insertvalue [10 x [10 x i32]] %.fca.1.9.insert584, i32 %.fca.2.0.load586, 2, 0
  %.fca.2.1.load589 = load i32, ptr %.fca.2.1.gep446, align 4
  %.fca.2.1.insert590 = insertvalue [10 x [10 x i32]] %.fca.2.0.insert587, i32 %.fca.2.1.load589, 2, 1
  %.fca.2.2.load592 = load i32, ptr %.fca.2.2.gep447, align 4
  %.fca.2.2.insert593 = insertvalue [10 x [10 x i32]] %.fca.2.1.insert590, i32 %.fca.2.2.load592, 2, 2
  %.fca.2.3.load595 = load i32, ptr %.fca.2.3.gep448, align 4
  %.fca.2.3.insert596 = insertvalue [10 x [10 x i32]] %.fca.2.2.insert593, i32 %.fca.2.3.load595, 2, 3
  %.fca.2.4.load598 = load i32, ptr %.fca.2.4.gep449, align 4
  %.fca.2.4.insert599 = insertvalue [10 x [10 x i32]] %.fca.2.3.insert596, i32 %.fca.2.4.load598, 2, 4
  %.fca.2.5.load601 = load i32, ptr %.fca.2.5.gep450, align 4
  %.fca.2.5.insert602 = insertvalue [10 x [10 x i32]] %.fca.2.4.insert599, i32 %.fca.2.5.load601, 2, 5
  %.fca.2.6.load604 = load i32, ptr %.fca.2.6.gep451, align 4
  %.fca.2.6.insert605 = insertvalue [10 x [10 x i32]] %.fca.2.5.insert602, i32 %.fca.2.6.load604, 2, 6
  %.fca.2.7.load607 = load i32, ptr %.fca.2.7.gep452, align 4
  %.fca.2.7.insert608 = insertvalue [10 x [10 x i32]] %.fca.2.6.insert605, i32 %.fca.2.7.load607, 2, 7
  %.fca.2.8.load610 = load i32, ptr %.fca.2.8.gep453, align 4
  %.fca.2.8.insert611 = insertvalue [10 x [10 x i32]] %.fca.2.7.insert608, i32 %.fca.2.8.load610, 2, 8
  %.fca.2.9.load613 = load i32, ptr %.fca.2.9.gep454, align 4
  %.fca.2.9.insert614 = insertvalue [10 x [10 x i32]] %.fca.2.8.insert611, i32 %.fca.2.9.load613, 2, 9
  %.fca.3.0.load616 = load i32, ptr %.fca.3.0.gep455, align 4
  %.fca.3.0.insert617 = insertvalue [10 x [10 x i32]] %.fca.2.9.insert614, i32 %.fca.3.0.load616, 3, 0
  %.fca.3.1.load619 = load i32, ptr %.fca.3.1.gep456, align 4
  %.fca.3.1.insert620 = insertvalue [10 x [10 x i32]] %.fca.3.0.insert617, i32 %.fca.3.1.load619, 3, 1
  %.fca.3.2.load622 = load i32, ptr %.fca.3.2.gep457, align 4
  %.fca.3.2.insert623 = insertvalue [10 x [10 x i32]] %.fca.3.1.insert620, i32 %.fca.3.2.load622, 3, 2
  %.fca.3.3.load625 = load i32, ptr %.fca.3.3.gep458, align 4
  %.fca.3.3.insert626 = insertvalue [10 x [10 x i32]] %.fca.3.2.insert623, i32 %.fca.3.3.load625, 3, 3
  %.fca.3.4.load628 = load i32, ptr %.fca.3.4.gep459, align 4
  %.fca.3.4.insert629 = insertvalue [10 x [10 x i32]] %.fca.3.3.insert626, i32 %.fca.3.4.load628, 3, 4
  %.fca.3.5.load631 = load i32, ptr %.fca.3.5.gep460, align 4
  %.fca.3.5.insert632 = insertvalue [10 x [10 x i32]] %.fca.3.4.insert629, i32 %.fca.3.5.load631, 3, 5
  %.fca.3.6.load634 = load i32, ptr %.fca.3.6.gep461, align 4
  %.fca.3.6.insert635 = insertvalue [10 x [10 x i32]] %.fca.3.5.insert632, i32 %.fca.3.6.load634, 3, 6
  %.fca.3.7.load637 = load i32, ptr %.fca.3.7.gep462, align 4
  %.fca.3.7.insert638 = insertvalue [10 x [10 x i32]] %.fca.3.6.insert635, i32 %.fca.3.7.load637, 3, 7
  %.fca.3.8.load640 = load i32, ptr %.fca.3.8.gep463, align 4
  %.fca.3.8.insert641 = insertvalue [10 x [10 x i32]] %.fca.3.7.insert638, i32 %.fca.3.8.load640, 3, 8
  %.fca.3.9.load643 = load i32, ptr %.fca.3.9.gep464, align 4
  %.fca.3.9.insert644 = insertvalue [10 x [10 x i32]] %.fca.3.8.insert641, i32 %.fca.3.9.load643, 3, 9
  %.fca.4.0.load646 = load i32, ptr %.fca.4.0.gep465, align 4
  %.fca.4.0.insert647 = insertvalue [10 x [10 x i32]] %.fca.3.9.insert644, i32 %.fca.4.0.load646, 4, 0
  %.fca.4.1.load649 = load i32, ptr %.fca.4.1.gep466, align 4
  %.fca.4.1.insert650 = insertvalue [10 x [10 x i32]] %.fca.4.0.insert647, i32 %.fca.4.1.load649, 4, 1
  %.fca.4.2.load652 = load i32, ptr %.fca.4.2.gep467, align 4
  %.fca.4.2.insert653 = insertvalue [10 x [10 x i32]] %.fca.4.1.insert650, i32 %.fca.4.2.load652, 4, 2
  %.fca.4.3.load655 = load i32, ptr %.fca.4.3.gep468, align 4
  %.fca.4.3.insert656 = insertvalue [10 x [10 x i32]] %.fca.4.2.insert653, i32 %.fca.4.3.load655, 4, 3
  %.fca.4.4.load658 = load i32, ptr %.fca.4.4.gep469, align 4
  %.fca.4.4.insert659 = insertvalue [10 x [10 x i32]] %.fca.4.3.insert656, i32 %.fca.4.4.load658, 4, 4
  %.fca.4.5.load661 = load i32, ptr %.fca.4.5.gep470, align 4
  %.fca.4.5.insert662 = insertvalue [10 x [10 x i32]] %.fca.4.4.insert659, i32 %.fca.4.5.load661, 4, 5
  %.fca.4.6.load664 = load i32, ptr %.fca.4.6.gep471, align 4
  %.fca.4.6.insert665 = insertvalue [10 x [10 x i32]] %.fca.4.5.insert662, i32 %.fca.4.6.load664, 4, 6
  %.fca.4.7.load667 = load i32, ptr %.fca.4.7.gep472, align 4
  %.fca.4.7.insert668 = insertvalue [10 x [10 x i32]] %.fca.4.6.insert665, i32 %.fca.4.7.load667, 4, 7
  %.fca.4.8.load670 = load i32, ptr %.fca.4.8.gep473, align 4
  %.fca.4.8.insert671 = insertvalue [10 x [10 x i32]] %.fca.4.7.insert668, i32 %.fca.4.8.load670, 4, 8
  %.fca.4.9.load673 = load i32, ptr %.fca.4.9.gep474, align 4
  %.fca.4.9.insert674 = insertvalue [10 x [10 x i32]] %.fca.4.8.insert671, i32 %.fca.4.9.load673, 4, 9
  %.fca.5.0.load676 = load i32, ptr %.fca.5.0.gep475, align 4
  %.fca.5.0.insert677 = insertvalue [10 x [10 x i32]] %.fca.4.9.insert674, i32 %.fca.5.0.load676, 5, 0
  %.fca.5.1.load679 = load i32, ptr %.fca.5.1.gep476, align 4
  %.fca.5.1.insert680 = insertvalue [10 x [10 x i32]] %.fca.5.0.insert677, i32 %.fca.5.1.load679, 5, 1
  %.fca.5.2.load682 = load i32, ptr %.fca.5.2.gep477, align 4
  %.fca.5.2.insert683 = insertvalue [10 x [10 x i32]] %.fca.5.1.insert680, i32 %.fca.5.2.load682, 5, 2
  %.fca.5.3.load685 = load i32, ptr %.fca.5.3.gep478, align 4
  %.fca.5.3.insert686 = insertvalue [10 x [10 x i32]] %.fca.5.2.insert683, i32 %.fca.5.3.load685, 5, 3
  %.fca.5.4.load688 = load i32, ptr %.fca.5.4.gep479, align 4
  %.fca.5.4.insert689 = insertvalue [10 x [10 x i32]] %.fca.5.3.insert686, i32 %.fca.5.4.load688, 5, 4
  %.fca.5.5.load691 = load i32, ptr %.fca.5.5.gep480, align 4
  %.fca.5.5.insert692 = insertvalue [10 x [10 x i32]] %.fca.5.4.insert689, i32 %.fca.5.5.load691, 5, 5
  %.fca.5.6.load694 = load i32, ptr %.fca.5.6.gep481, align 4
  %.fca.5.6.insert695 = insertvalue [10 x [10 x i32]] %.fca.5.5.insert692, i32 %.fca.5.6.load694, 5, 6
  %.fca.5.7.load697 = load i32, ptr %.fca.5.7.gep482, align 4
  %.fca.5.7.insert698 = insertvalue [10 x [10 x i32]] %.fca.5.6.insert695, i32 %.fca.5.7.load697, 5, 7
  %.fca.5.8.load700 = load i32, ptr %.fca.5.8.gep483, align 4
  %.fca.5.8.insert701 = insertvalue [10 x [10 x i32]] %.fca.5.7.insert698, i32 %.fca.5.8.load700, 5, 8
  %.fca.5.9.load703 = load i32, ptr %.fca.5.9.gep484, align 4
  %.fca.5.9.insert704 = insertvalue [10 x [10 x i32]] %.fca.5.8.insert701, i32 %.fca.5.9.load703, 5, 9
  %.fca.6.0.load706 = load i32, ptr %.fca.6.0.gep485, align 4
  %.fca.6.0.insert707 = insertvalue [10 x [10 x i32]] %.fca.5.9.insert704, i32 %.fca.6.0.load706, 6, 0
  %.fca.6.1.load709 = load i32, ptr %.fca.6.1.gep486, align 4
  %.fca.6.1.insert710 = insertvalue [10 x [10 x i32]] %.fca.6.0.insert707, i32 %.fca.6.1.load709, 6, 1
  %.fca.6.2.load712 = load i32, ptr %.fca.6.2.gep487, align 4
  %.fca.6.2.insert713 = insertvalue [10 x [10 x i32]] %.fca.6.1.insert710, i32 %.fca.6.2.load712, 6, 2
  %.fca.6.3.load715 = load i32, ptr %.fca.6.3.gep488, align 4
  %.fca.6.3.insert716 = insertvalue [10 x [10 x i32]] %.fca.6.2.insert713, i32 %.fca.6.3.load715, 6, 3
  %.fca.6.4.load718 = load i32, ptr %.fca.6.4.gep489, align 4
  %.fca.6.4.insert719 = insertvalue [10 x [10 x i32]] %.fca.6.3.insert716, i32 %.fca.6.4.load718, 6, 4
  %.fca.6.5.load721 = load i32, ptr %.fca.6.5.gep490, align 4
  %.fca.6.5.insert722 = insertvalue [10 x [10 x i32]] %.fca.6.4.insert719, i32 %.fca.6.5.load721, 6, 5
  %.fca.6.6.load724 = load i32, ptr %.fca.6.6.gep491, align 4
  %.fca.6.6.insert725 = insertvalue [10 x [10 x i32]] %.fca.6.5.insert722, i32 %.fca.6.6.load724, 6, 6
  %.fca.6.7.load727 = load i32, ptr %.fca.6.7.gep492, align 4
  %.fca.6.7.insert728 = insertvalue [10 x [10 x i32]] %.fca.6.6.insert725, i32 %.fca.6.7.load727, 6, 7
  %.fca.6.8.load730 = load i32, ptr %.fca.6.8.gep493, align 4
  %.fca.6.8.insert731 = insertvalue [10 x [10 x i32]] %.fca.6.7.insert728, i32 %.fca.6.8.load730, 6, 8
  %.fca.6.9.load733 = load i32, ptr %.fca.6.9.gep494, align 4
  %.fca.6.9.insert734 = insertvalue [10 x [10 x i32]] %.fca.6.8.insert731, i32 %.fca.6.9.load733, 6, 9
  %.fca.7.0.load736 = load i32, ptr %.fca.7.0.gep495, align 4
  %.fca.7.0.insert737 = insertvalue [10 x [10 x i32]] %.fca.6.9.insert734, i32 %.fca.7.0.load736, 7, 0
  %.fca.7.1.load739 = load i32, ptr %.fca.7.1.gep496, align 4
  %.fca.7.1.insert740 = insertvalue [10 x [10 x i32]] %.fca.7.0.insert737, i32 %.fca.7.1.load739, 7, 1
  %.fca.7.2.load742 = load i32, ptr %.fca.7.2.gep497, align 4
  %.fca.7.2.insert743 = insertvalue [10 x [10 x i32]] %.fca.7.1.insert740, i32 %.fca.7.2.load742, 7, 2
  %.fca.7.3.load745 = load i32, ptr %.fca.7.3.gep498, align 4
  %.fca.7.3.insert746 = insertvalue [10 x [10 x i32]] %.fca.7.2.insert743, i32 %.fca.7.3.load745, 7, 3
  %.fca.7.4.load748 = load i32, ptr %.fca.7.4.gep499, align 4
  %.fca.7.4.insert749 = insertvalue [10 x [10 x i32]] %.fca.7.3.insert746, i32 %.fca.7.4.load748, 7, 4
  %.fca.7.5.load751 = load i32, ptr %.fca.7.5.gep500, align 4
  %.fca.7.5.insert752 = insertvalue [10 x [10 x i32]] %.fca.7.4.insert749, i32 %.fca.7.5.load751, 7, 5
  %.fca.7.6.load754 = load i32, ptr %.fca.7.6.gep501, align 4
  %.fca.7.6.insert755 = insertvalue [10 x [10 x i32]] %.fca.7.5.insert752, i32 %.fca.7.6.load754, 7, 6
  %.fca.7.7.load757 = load i32, ptr %.fca.7.7.gep502, align 4
  %.fca.7.7.insert758 = insertvalue [10 x [10 x i32]] %.fca.7.6.insert755, i32 %.fca.7.7.load757, 7, 7
  %.fca.7.8.load760 = load i32, ptr %.fca.7.8.gep503, align 4
  %.fca.7.8.insert761 = insertvalue [10 x [10 x i32]] %.fca.7.7.insert758, i32 %.fca.7.8.load760, 7, 8
  %.fca.7.9.load763 = load i32, ptr %.fca.7.9.gep504, align 4
  %.fca.7.9.insert764 = insertvalue [10 x [10 x i32]] %.fca.7.8.insert761, i32 %.fca.7.9.load763, 7, 9
  %.fca.8.0.load766 = load i32, ptr %.fca.8.0.gep505, align 4
  %.fca.8.0.insert767 = insertvalue [10 x [10 x i32]] %.fca.7.9.insert764, i32 %.fca.8.0.load766, 8, 0
  %.fca.8.1.load769 = load i32, ptr %.fca.8.1.gep506, align 4
  %.fca.8.1.insert770 = insertvalue [10 x [10 x i32]] %.fca.8.0.insert767, i32 %.fca.8.1.load769, 8, 1
  %.fca.8.2.load772 = load i32, ptr %.fca.8.2.gep507, align 4
  %.fca.8.2.insert773 = insertvalue [10 x [10 x i32]] %.fca.8.1.insert770, i32 %.fca.8.2.load772, 8, 2
  %.fca.8.3.load775 = load i32, ptr %.fca.8.3.gep508, align 4
  %.fca.8.3.insert776 = insertvalue [10 x [10 x i32]] %.fca.8.2.insert773, i32 %.fca.8.3.load775, 8, 3
  %.fca.8.4.load778 = load i32, ptr %.fca.8.4.gep509, align 4
  %.fca.8.4.insert779 = insertvalue [10 x [10 x i32]] %.fca.8.3.insert776, i32 %.fca.8.4.load778, 8, 4
  %.fca.8.5.load781 = load i32, ptr %.fca.8.5.gep510, align 4
  %.fca.8.5.insert782 = insertvalue [10 x [10 x i32]] %.fca.8.4.insert779, i32 %.fca.8.5.load781, 8, 5
  %.fca.8.6.load784 = load i32, ptr %.fca.8.6.gep511, align 4
  %.fca.8.6.insert785 = insertvalue [10 x [10 x i32]] %.fca.8.5.insert782, i32 %.fca.8.6.load784, 8, 6
  %.fca.8.7.load787 = load i32, ptr %.fca.8.7.gep512, align 4
  %.fca.8.7.insert788 = insertvalue [10 x [10 x i32]] %.fca.8.6.insert785, i32 %.fca.8.7.load787, 8, 7
  %.fca.8.8.load790 = load i32, ptr %.fca.8.8.gep513, align 4
  %.fca.8.8.insert791 = insertvalue [10 x [10 x i32]] %.fca.8.7.insert788, i32 %.fca.8.8.load790, 8, 8
  %.fca.8.9.load793 = load i32, ptr %.fca.8.9.gep514, align 4
  %.fca.8.9.insert794 = insertvalue [10 x [10 x i32]] %.fca.8.8.insert791, i32 %.fca.8.9.load793, 8, 9
  %.fca.9.0.load796 = load i32, ptr %.fca.9.0.gep515, align 4
  %.fca.9.0.insert797 = insertvalue [10 x [10 x i32]] %.fca.8.9.insert794, i32 %.fca.9.0.load796, 9, 0
  %.fca.9.1.load799 = load i32, ptr %.fca.9.1.gep516, align 4
  %.fca.9.1.insert800 = insertvalue [10 x [10 x i32]] %.fca.9.0.insert797, i32 %.fca.9.1.load799, 9, 1
  %.fca.9.2.load802 = load i32, ptr %.fca.9.2.gep517, align 4
  %.fca.9.2.insert803 = insertvalue [10 x [10 x i32]] %.fca.9.1.insert800, i32 %.fca.9.2.load802, 9, 2
  %.fca.9.3.load805 = load i32, ptr %.fca.9.3.gep518, align 4
  %.fca.9.3.insert806 = insertvalue [10 x [10 x i32]] %.fca.9.2.insert803, i32 %.fca.9.3.load805, 9, 3
  %.fca.9.4.load808 = load i32, ptr %.fca.9.4.gep519, align 4
  %.fca.9.4.insert809 = insertvalue [10 x [10 x i32]] %.fca.9.3.insert806, i32 %.fca.9.4.load808, 9, 4
  %.fca.9.5.load811 = load i32, ptr %.fca.9.5.gep520, align 4
  %.fca.9.5.insert812 = insertvalue [10 x [10 x i32]] %.fca.9.4.insert809, i32 %.fca.9.5.load811, 9, 5
  %.fca.9.6.load814 = load i32, ptr %.fca.9.6.gep521, align 4
  %.fca.9.6.insert815 = insertvalue [10 x [10 x i32]] %.fca.9.5.insert812, i32 %.fca.9.6.load814, 9, 6
  %.fca.9.7.load817 = load i32, ptr %.fca.9.7.gep522, align 4
  %.fca.9.7.insert818 = insertvalue [10 x [10 x i32]] %.fca.9.6.insert815, i32 %.fca.9.7.load817, 9, 7
  %.fca.9.8.load820 = load i32, ptr %.fca.9.8.gep523, align 4
  %.fca.9.8.insert821 = insertvalue [10 x [10 x i32]] %.fca.9.7.insert818, i32 %.fca.9.8.load820, 9, 8
  %.fca.9.9.load823 = load i32, ptr %.fca.9.9.gep524, align 4
  %.fca.9.9.insert824 = insertvalue [10 x [10 x i32]] %.fca.9.8.insert821, i32 %.fca.9.9.load823, 9, 9
  tail call fastcc void @_p__void__void__printGeneration__string_intarrayarray(ptr nonnull @anon.string.0, [10 x [10 x i32]] %.fca.9.9.insert824) #5
  %.fca.0.4.load838.pre = load i32, ptr %.fca.0.4.gep429, align 4
  %.fca.0.5.load841.pre = load i32, ptr %.fca.0.5.gep430, align 4
  %.fca.0.6.load844.pre = load i32, ptr %.fca.0.6.gep431, align 4
  %.fca.0.7.load847.pre = load i32, ptr %.fca.0.7.gep432, align 4
  %.fca.0.8.load850.pre = load i32, ptr %.fca.0.8.gep433, align 4
  %.fca.0.9.load853.pre = load i32, ptr %.fca.0.9.gep434, align 4
  %.fca.1.0.load856.pre = load i32, ptr %.fca.1.0.gep435, align 4
  %.fca.1.1.load859.pre = load i32, ptr %.fca.1.1.gep436, align 4
  %.fca.1.2.load862.pre = load i32, ptr %.fca.1.2.gep437, align 4
  %.fca.1.3.load865.pre = load i32, ptr %.fca.1.3.gep438, align 4
  %.fca.1.4.load868.pre = load i32, ptr %.fca.1.4.gep439, align 4
  %.fca.1.5.load871.pre = load i32, ptr %.fca.1.5.gep440, align 4
  %.fca.1.6.load874.pre = load i32, ptr %.fca.1.6.gep441, align 4
  %.fca.1.7.load877.pre = load i32, ptr %.fca.1.7.gep442, align 4
  %.fca.1.8.load880.pre = load i32, ptr %.fca.1.8.gep443, align 4
  %.fca.1.9.load883.pre = load i32, ptr %.fca.1.9.gep444, align 4
  %.fca.2.0.load886.pre = load i32, ptr %.fca.2.0.gep445, align 4
  %.fca.2.1.load889.pre = load i32, ptr %.fca.2.1.gep446, align 4
  %.fca.2.2.load892.pre = load i32, ptr %.fca.2.2.gep447, align 4
  %.fca.2.3.load895.pre = load i32, ptr %.fca.2.3.gep448, align 4
  %.fca.2.4.load898.pre = load i32, ptr %.fca.2.4.gep449, align 4
  %.fca.2.5.load901.pre = load i32, ptr %.fca.2.5.gep450, align 4
  %.fca.2.6.load904.pre = load i32, ptr %.fca.2.6.gep451, align 4
  %.fca.2.7.load907.pre = load i32, ptr %.fca.2.7.gep452, align 4
  %.fca.2.8.load910.pre = load i32, ptr %.fca.2.8.gep453, align 4
  %.fca.2.9.load913.pre = load i32, ptr %.fca.2.9.gep454, align 4
  %.fca.3.0.load916.pre = load i32, ptr %.fca.3.0.gep455, align 4
  %.fca.3.1.load919.pre = load i32, ptr %.fca.3.1.gep456, align 4
  %.fca.3.2.load922.pre = load i32, ptr %.fca.3.2.gep457, align 4
  %.fca.3.3.load925.pre = load i32, ptr %.fca.3.3.gep458, align 4
  %.fca.3.4.load928.pre = load i32, ptr %.fca.3.4.gep459, align 4
  %.fca.3.5.load931.pre = load i32, ptr %.fca.3.5.gep460, align 4
  %.fca.3.6.load934.pre = load i32, ptr %.fca.3.6.gep461, align 4
  %.fca.3.7.load937.pre = load i32, ptr %.fca.3.7.gep462, align 4
  %.fca.3.8.load940.pre = load i32, ptr %.fca.3.8.gep463, align 4
  %.fca.3.9.load943.pre = load i32, ptr %.fca.3.9.gep464, align 4
  %.fca.4.0.load946.pre = load i32, ptr %.fca.4.0.gep465, align 4
  %.fca.4.1.load949.pre = load i32, ptr %.fca.4.1.gep466, align 4
  %.fca.4.2.load952.pre = load i32, ptr %.fca.4.2.gep467, align 4
  %.fca.4.3.load955.pre = load i32, ptr %.fca.4.3.gep468, align 4
  %.fca.4.4.load958.pre = load i32, ptr %.fca.4.4.gep469, align 4
  %.fca.4.5.load961.pre = load i32, ptr %.fca.4.5.gep470, align 4
  %.fca.4.6.load964.pre = load i32, ptr %.fca.4.6.gep471, align 4
  %.fca.4.7.load967.pre = load i32, ptr %.fca.4.7.gep472, align 4
  %.fca.4.8.load970.pre = load i32, ptr %.fca.4.8.gep473, align 4
  %.fca.4.9.load973.pre = load i32, ptr %.fca.4.9.gep474, align 4
  br label %for.head.L73.preheader

for.head.L73.preheader:                           ; preds = %for.exit.L64, %for.exit.L73
  %.fca.4.9.load973 = phi i32 [ %.fca.4.9.load973.pre, %for.exit.L64 ], [ %.fca.4.9.load273, %for.exit.L73 ]
  %.fca.4.8.load970 = phi i32 [ %.fca.4.8.load970.pre, %for.exit.L64 ], [ %.fca.4.8.load270, %for.exit.L73 ]
  %.fca.4.7.load967 = phi i32 [ %.fca.4.7.load967.pre, %for.exit.L64 ], [ %.fca.4.7.load267, %for.exit.L73 ]
  %.fca.4.6.load964 = phi i32 [ %.fca.4.6.load964.pre, %for.exit.L64 ], [ %.fca.4.6.load264, %for.exit.L73 ]
  %.fca.4.5.load961 = phi i32 [ %.fca.4.5.load961.pre, %for.exit.L64 ], [ %.fca.4.5.load261, %for.exit.L73 ]
  %.fca.4.4.load958 = phi i32 [ %.fca.4.4.load958.pre, %for.exit.L64 ], [ %.fca.4.4.load258, %for.exit.L73 ]
  %.fca.4.3.load955 = phi i32 [ %.fca.4.3.load955.pre, %for.exit.L64 ], [ %.fca.4.3.load255, %for.exit.L73 ]
  %.fca.4.2.load952 = phi i32 [ %.fca.4.2.load952.pre, %for.exit.L64 ], [ %.fca.4.2.load252, %for.exit.L73 ]
  %.fca.4.1.load949 = phi i32 [ %.fca.4.1.load949.pre, %for.exit.L64 ], [ %.fca.4.1.load249, %for.exit.L73 ]
  %.fca.4.0.load946 = phi i32 [ %.fca.4.0.load946.pre, %for.exit.L64 ], [ %.fca.4.0.load246, %for.exit.L73 ]
  %.fca.3.9.load943 = phi i32 [ %.fca.3.9.load943.pre, %for.exit.L64 ], [ %.fca.3.9.load243, %for.exit.L73 ]
  %.fca.3.8.load940 = phi i32 [ %.fca.3.8.load940.pre, %for.exit.L64 ], [ %.fca.3.8.load240, %for.exit.L73 ]
  %.fca.3.7.load937 = phi i32 [ %.fca.3.7.load937.pre, %for.exit.L64 ], [ %.fca.3.7.load237, %for.exit.L73 ]
  %.fca.3.6.load934 = phi i32 [ %.fca.3.6.load934.pre, %for.exit.L64 ], [ %.fca.3.6.load234, %for.exit.L73 ]
  %.fca.3.5.load931 = phi i32 [ %.fca.3.5.load931.pre, %for.exit.L64 ], [ %.fca.3.5.load231, %for.exit.L73 ]
  %.fca.3.4.load928 = phi i32 [ %.fca.3.4.load928.pre, %for.exit.L64 ], [ %.fca.3.4.load228, %for.exit.L73 ]
  %.fca.3.3.load925 = phi i32 [ %.fca.3.3.load925.pre, %for.exit.L64 ], [ %.fca.3.3.load225, %for.exit.L73 ]
  %.fca.3.2.load922 = phi i32 [ %.fca.3.2.load922.pre, %for.exit.L64 ], [ %.fca.3.2.load222, %for.exit.L73 ]
  %.fca.3.1.load919 = phi i32 [ %.fca.3.1.load919.pre, %for.exit.L64 ], [ %.fca.3.1.load219, %for.exit.L73 ]
  %.fca.3.0.load916 = phi i32 [ %.fca.3.0.load916.pre, %for.exit.L64 ], [ %.fca.3.0.load216, %for.exit.L73 ]
  %.fca.2.9.load913 = phi i32 [ %.fca.2.9.load913.pre, %for.exit.L64 ], [ %.fca.2.9.load213, %for.exit.L73 ]
  %.fca.2.8.load910 = phi i32 [ %.fca.2.8.load910.pre, %for.exit.L64 ], [ %.fca.2.8.load210, %for.exit.L73 ]
  %.fca.2.7.load907 = phi i32 [ %.fca.2.7.load907.pre, %for.exit.L64 ], [ %.fca.2.7.load207, %for.exit.L73 ]
  %.fca.2.6.load904 = phi i32 [ %.fca.2.6.load904.pre, %for.exit.L64 ], [ %.fca.2.6.load204, %for.exit.L73 ]
  %.fca.2.5.load901 = phi i32 [ %.fca.2.5.load901.pre, %for.exit.L64 ], [ %.fca.2.5.load201, %for.exit.L73 ]
  %.fca.2.4.load898 = phi i32 [ %.fca.2.4.load898.pre, %for.exit.L64 ], [ %.fca.2.4.load198, %for.exit.L73 ]
  %.fca.2.3.load895 = phi i32 [ %.fca.2.3.load895.pre, %for.exit.L64 ], [ %.fca.2.3.load195, %for.exit.L73 ]
  %.fca.2.2.load892 = phi i32 [ %.fca.2.2.load892.pre, %for.exit.L64 ], [ %.fca.2.2.load192, %for.exit.L73 ]
  %.fca.2.1.load889 = phi i32 [ %.fca.2.1.load889.pre, %for.exit.L64 ], [ %.fca.2.1.load189, %for.exit.L73 ]
  %.fca.2.0.load886 = phi i32 [ %.fca.2.0.load886.pre, %for.exit.L64 ], [ %.fca.2.0.load186, %for.exit.L73 ]
  %.fca.1.9.load883 = phi i32 [ %.fca.1.9.load883.pre, %for.exit.L64 ], [ %.fca.1.9.load183, %for.exit.L73 ]
  %.fca.1.8.load880 = phi i32 [ %.fca.1.8.load880.pre, %for.exit.L64 ], [ %.fca.1.8.load180, %for.exit.L73 ]
  %.fca.1.7.load877 = phi i32 [ %.fca.1.7.load877.pre, %for.exit.L64 ], [ %.fca.1.7.load177, %for.exit.L73 ]
  %.fca.1.6.load874 = phi i32 [ %.fca.1.6.load874.pre, %for.exit.L64 ], [ %.fca.1.6.load174, %for.exit.L73 ]
  %.fca.1.5.load871 = phi i32 [ %.fca.1.5.load871.pre, %for.exit.L64 ], [ %.fca.1.5.load171, %for.exit.L73 ]
  %.fca.1.4.load868 = phi i32 [ %.fca.1.4.load868.pre, %for.exit.L64 ], [ %.fca.1.4.load168, %for.exit.L73 ]
  %.fca.1.3.load865 = phi i32 [ %.fca.1.3.load865.pre, %for.exit.L64 ], [ %.fca.1.3.load165, %for.exit.L73 ]
  %.fca.1.2.load862 = phi i32 [ %.fca.1.2.load862.pre, %for.exit.L64 ], [ %.fca.1.2.load162, %for.exit.L73 ]
  %.fca.1.1.load859 = phi i32 [ %.fca.1.1.load859.pre, %for.exit.L64 ], [ %.fca.1.1.load159, %for.exit.L73 ]
  %.fca.1.0.load856 = phi i32 [ %.fca.1.0.load856.pre, %for.exit.L64 ], [ %.fca.1.0.load156, %for.exit.L73 ]
  %.fca.0.9.load853 = phi i32 [ %.fca.0.9.load853.pre, %for.exit.L64 ], [ %.fca.0.9.load153, %for.exit.L73 ]
  %.fca.0.8.load850 = phi i32 [ %.fca.0.8.load850.pre, %for.exit.L64 ], [ %.fca.0.8.load150, %for.exit.L73 ]
  %.fca.0.7.load847 = phi i32 [ %.fca.0.7.load847.pre, %for.exit.L64 ], [ %.fca.0.7.load147, %for.exit.L73 ]
  %.fca.0.6.load844 = phi i32 [ %.fca.0.6.load844.pre, %for.exit.L64 ], [ %.fca.0.6.load144, %for.exit.L73 ]
  %.fca.0.5.load841 = phi i32 [ %.fca.0.5.load841.pre, %for.exit.L64 ], [ %.fca.0.5.load141, %for.exit.L73 ]
  %.fca.0.4.load838 = phi i32 [ %.fca.0.4.load838.pre, %for.exit.L64 ], [ %.fca.0.4.load138, %for.exit.L73 ]
  %generation.01129 = phi i32 [ 1, %for.exit.L64 ], [ %32, %for.exit.L73 ]
  %.fca.0.0.load826 = load i32, ptr %a, align 4
  %.fca.0.0.insert827 = insertvalue [10 x [10 x i32]] poison, i32 %.fca.0.0.load826, 0, 0
  %.fca.0.1.load829 = load i32, ptr %.fca.0.1.gep426, align 4
  %.fca.0.1.insert830 = insertvalue [10 x [10 x i32]] %.fca.0.0.insert827, i32 %.fca.0.1.load829, 0, 1
  %.fca.0.2.load832 = load i32, ptr %.fca.0.2.gep427, align 4
  %.fca.0.2.insert833 = insertvalue [10 x [10 x i32]] %.fca.0.1.insert830, i32 %.fca.0.2.load832, 0, 2
  %.fca.0.3.load835 = load i32, ptr %.fca.0.3.gep428, align 4
  %.fca.0.3.insert836 = insertvalue [10 x [10 x i32]] %.fca.0.2.insert833, i32 %.fca.0.3.load835, 0, 3
  %.fca.0.4.insert839 = insertvalue [10 x [10 x i32]] %.fca.0.3.insert836, i32 %.fca.0.4.load838, 0, 4
  %.fca.0.5.insert842 = insertvalue [10 x [10 x i32]] %.fca.0.4.insert839, i32 %.fca.0.5.load841, 0, 5
  %.fca.0.6.insert845 = insertvalue [10 x [10 x i32]] %.fca.0.5.insert842, i32 %.fca.0.6.load844, 0, 6
  %.fca.0.7.insert848 = insertvalue [10 x [10 x i32]] %.fca.0.6.insert845, i32 %.fca.0.7.load847, 0, 7
  %.fca.0.8.insert851 = insertvalue [10 x [10 x i32]] %.fca.0.7.insert848, i32 %.fca.0.8.load850, 0, 8
  %.fca.0.9.insert854 = insertvalue [10 x [10 x i32]] %.fca.0.8.insert851, i32 %.fca.0.9.load853, 0, 9
  %.fca.1.0.insert857 = insertvalue [10 x [10 x i32]] %.fca.0.9.insert854, i32 %.fca.1.0.load856, 1, 0
  %.fca.1.1.insert860 = insertvalue [10 x [10 x i32]] %.fca.1.0.insert857, i32 %.fca.1.1.load859, 1, 1
  %.fca.1.2.insert863 = insertvalue [10 x [10 x i32]] %.fca.1.1.insert860, i32 %.fca.1.2.load862, 1, 2
  %.fca.1.3.insert866 = insertvalue [10 x [10 x i32]] %.fca.1.2.insert863, i32 %.fca.1.3.load865, 1, 3
  %.fca.1.4.insert869 = insertvalue [10 x [10 x i32]] %.fca.1.3.insert866, i32 %.fca.1.4.load868, 1, 4
  %.fca.1.5.insert872 = insertvalue [10 x [10 x i32]] %.fca.1.4.insert869, i32 %.fca.1.5.load871, 1, 5
  %.fca.1.6.insert875 = insertvalue [10 x [10 x i32]] %.fca.1.5.insert872, i32 %.fca.1.6.load874, 1, 6
  %.fca.1.7.insert878 = insertvalue [10 x [10 x i32]] %.fca.1.6.insert875, i32 %.fca.1.7.load877, 1, 7
  %.fca.1.8.insert881 = insertvalue [10 x [10 x i32]] %.fca.1.7.insert878, i32 %.fca.1.8.load880, 1, 8
  %.fca.1.9.insert884 = insertvalue [10 x [10 x i32]] %.fca.1.8.insert881, i32 %.fca.1.9.load883, 1, 9
  %.fca.2.0.insert887 = insertvalue [10 x [10 x i32]] %.fca.1.9.insert884, i32 %.fca.2.0.load886, 2, 0
  %.fca.2.1.insert890 = insertvalue [10 x [10 x i32]] %.fca.2.0.insert887, i32 %.fca.2.1.load889, 2, 1
  %.fca.2.2.insert893 = insertvalue [10 x [10 x i32]] %.fca.2.1.insert890, i32 %.fca.2.2.load892, 2, 2
  %.fca.2.3.insert896 = insertvalue [10 x [10 x i32]] %.fca.2.2.insert893, i32 %.fca.2.3.load895, 2, 3
  %.fca.2.4.insert899 = insertvalue [10 x [10 x i32]] %.fca.2.3.insert896, i32 %.fca.2.4.load898, 2, 4
  %.fca.2.5.insert902 = insertvalue [10 x [10 x i32]] %.fca.2.4.insert899, i32 %.fca.2.5.load901, 2, 5
  %.fca.2.6.insert905 = insertvalue [10 x [10 x i32]] %.fca.2.5.insert902, i32 %.fca.2.6.load904, 2, 6
  %.fca.2.7.insert908 = insertvalue [10 x [10 x i32]] %.fca.2.6.insert905, i32 %.fca.2.7.load907, 2, 7
  %.fca.2.8.insert911 = insertvalue [10 x [10 x i32]] %.fca.2.7.insert908, i32 %.fca.2.8.load910, 2, 8
  %.fca.2.9.insert914 = insertvalue [10 x [10 x i32]] %.fca.2.8.insert911, i32 %.fca.2.9.load913, 2, 9
  %.fca.3.0.insert917 = insertvalue [10 x [10 x i32]] %.fca.2.9.insert914, i32 %.fca.3.0.load916, 3, 0
  %.fca.3.1.insert920 = insertvalue [10 x [10 x i32]] %.fca.3.0.insert917, i32 %.fca.3.1.load919, 3, 1
  %.fca.3.2.insert923 = insertvalue [10 x [10 x i32]] %.fca.3.1.insert920, i32 %.fca.3.2.load922, 3, 2
  %.fca.3.3.insert926 = insertvalue [10 x [10 x i32]] %.fca.3.2.insert923, i32 %.fca.3.3.load925, 3, 3
  %.fca.3.4.insert929 = insertvalue [10 x [10 x i32]] %.fca.3.3.insert926, i32 %.fca.3.4.load928, 3, 4
  %.fca.3.5.insert932 = insertvalue [10 x [10 x i32]] %.fca.3.4.insert929, i32 %.fca.3.5.load931, 3, 5
  %.fca.3.6.insert935 = insertvalue [10 x [10 x i32]] %.fca.3.5.insert932, i32 %.fca.3.6.load934, 3, 6
  %.fca.3.7.insert938 = insertvalue [10 x [10 x i32]] %.fca.3.6.insert935, i32 %.fca.3.7.load937, 3, 7
  %.fca.3.8.insert941 = insertvalue [10 x [10 x i32]] %.fca.3.7.insert938, i32 %.fca.3.8.load940, 3, 8
  %.fca.3.9.insert944 = insertvalue [10 x [10 x i32]] %.fca.3.8.insert941, i32 %.fca.3.9.load943, 3, 9
  %.fca.4.0.insert947 = insertvalue [10 x [10 x i32]] %.fca.3.9.insert944, i32 %.fca.4.0.load946, 4, 0
  %.fca.4.1.insert950 = insertvalue [10 x [10 x i32]] %.fca.4.0.insert947, i32 %.fca.4.1.load949, 4, 1
  %.fca.4.2.insert953 = insertvalue [10 x [10 x i32]] %.fca.4.1.insert950, i32 %.fca.4.2.load952, 4, 2
  %.fca.4.3.insert956 = insertvalue [10 x [10 x i32]] %.fca.4.2.insert953, i32 %.fca.4.3.load955, 4, 3
  %.fca.4.4.insert959 = insertvalue [10 x [10 x i32]] %.fca.4.3.insert956, i32 %.fca.4.4.load958, 4, 4
  %.fca.4.5.insert962 = insertvalue [10 x [10 x i32]] %.fca.4.4.insert959, i32 %.fca.4.5.load961, 4, 5
  %.fca.4.6.insert965 = insertvalue [10 x [10 x i32]] %.fca.4.5.insert962, i32 %.fca.4.6.load964, 4, 6
  %.fca.4.7.insert968 = insertvalue [10 x [10 x i32]] %.fca.4.6.insert965, i32 %.fca.4.7.load967, 4, 7
  %.fca.4.8.insert971 = insertvalue [10 x [10 x i32]] %.fca.4.7.insert968, i32 %.fca.4.8.load970, 4, 8
  %.fca.4.9.insert974 = insertvalue [10 x [10 x i32]] %.fca.4.8.insert971, i32 %.fca.4.9.load973, 4, 9
  %.fca.5.0.load976 = load i32, ptr %.fca.5.0.gep475, align 4
  %.fca.5.0.insert977 = insertvalue [10 x [10 x i32]] %.fca.4.9.insert974, i32 %.fca.5.0.load976, 5, 0
  %.fca.5.1.load979 = load i32, ptr %.fca.5.1.gep476, align 4
  %.fca.5.1.insert980 = insertvalue [10 x [10 x i32]] %.fca.5.0.insert977, i32 %.fca.5.1.load979, 5, 1
  %.fca.5.2.load982 = load i32, ptr %.fca.5.2.gep477, align 4
  %.fca.5.2.insert983 = insertvalue [10 x [10 x i32]] %.fca.5.1.insert980, i32 %.fca.5.2.load982, 5, 2
  %.fca.5.3.load985 = load i32, ptr %.fca.5.3.gep478, align 4
  %.fca.5.3.insert986 = insertvalue [10 x [10 x i32]] %.fca.5.2.insert983, i32 %.fca.5.3.load985, 5, 3
  %.fca.5.4.load988 = load i32, ptr %.fca.5.4.gep479, align 4
  %.fca.5.4.insert989 = insertvalue [10 x [10 x i32]] %.fca.5.3.insert986, i32 %.fca.5.4.load988, 5, 4
  %.fca.5.5.load991 = load i32, ptr %.fca.5.5.gep480, align 4
  %.fca.5.5.insert992 = insertvalue [10 x [10 x i32]] %.fca.5.4.insert989, i32 %.fca.5.5.load991, 5, 5
  %.fca.5.6.load994 = load i32, ptr %.fca.5.6.gep481, align 4
  %.fca.5.6.insert995 = insertvalue [10 x [10 x i32]] %.fca.5.5.insert992, i32 %.fca.5.6.load994, 5, 6
  %.fca.5.7.load997 = load i32, ptr %.fca.5.7.gep482, align 4
  %.fca.5.7.insert998 = insertvalue [10 x [10 x i32]] %.fca.5.6.insert995, i32 %.fca.5.7.load997, 5, 7
  %.fca.5.8.load1000 = load i32, ptr %.fca.5.8.gep483, align 4
  %.fca.5.8.insert1001 = insertvalue [10 x [10 x i32]] %.fca.5.7.insert998, i32 %.fca.5.8.load1000, 5, 8
  %.fca.5.9.load1003 = load i32, ptr %.fca.5.9.gep484, align 4
  %.fca.5.9.insert1004 = insertvalue [10 x [10 x i32]] %.fca.5.8.insert1001, i32 %.fca.5.9.load1003, 5, 9
  %.fca.6.0.load1006 = load i32, ptr %.fca.6.0.gep485, align 4
  %.fca.6.0.insert1007 = insertvalue [10 x [10 x i32]] %.fca.5.9.insert1004, i32 %.fca.6.0.load1006, 6, 0
  %.fca.6.1.load1009 = load i32, ptr %.fca.6.1.gep486, align 4
  %.fca.6.1.insert1010 = insertvalue [10 x [10 x i32]] %.fca.6.0.insert1007, i32 %.fca.6.1.load1009, 6, 1
  %.fca.6.2.load1012 = load i32, ptr %.fca.6.2.gep487, align 4
  %.fca.6.2.insert1013 = insertvalue [10 x [10 x i32]] %.fca.6.1.insert1010, i32 %.fca.6.2.load1012, 6, 2
  %.fca.6.3.load1015 = load i32, ptr %.fca.6.3.gep488, align 4
  %.fca.6.3.insert1016 = insertvalue [10 x [10 x i32]] %.fca.6.2.insert1013, i32 %.fca.6.3.load1015, 6, 3
  %.fca.6.4.load1018 = load i32, ptr %.fca.6.4.gep489, align 4
  %.fca.6.4.insert1019 = insertvalue [10 x [10 x i32]] %.fca.6.3.insert1016, i32 %.fca.6.4.load1018, 6, 4
  %.fca.6.5.load1021 = load i32, ptr %.fca.6.5.gep490, align 4
  %.fca.6.5.insert1022 = insertvalue [10 x [10 x i32]] %.fca.6.4.insert1019, i32 %.fca.6.5.load1021, 6, 5
  %.fca.6.6.load1024 = load i32, ptr %.fca.6.6.gep491, align 4
  %.fca.6.6.insert1025 = insertvalue [10 x [10 x i32]] %.fca.6.5.insert1022, i32 %.fca.6.6.load1024, 6, 6
  %.fca.6.7.load1027 = load i32, ptr %.fca.6.7.gep492, align 4
  %.fca.6.7.insert1028 = insertvalue [10 x [10 x i32]] %.fca.6.6.insert1025, i32 %.fca.6.7.load1027, 6, 7
  %.fca.6.8.load1030 = load i32, ptr %.fca.6.8.gep493, align 4
  %.fca.6.8.insert1031 = insertvalue [10 x [10 x i32]] %.fca.6.7.insert1028, i32 %.fca.6.8.load1030, 6, 8
  %.fca.6.9.load1033 = load i32, ptr %.fca.6.9.gep494, align 4
  %.fca.6.9.insert1034 = insertvalue [10 x [10 x i32]] %.fca.6.8.insert1031, i32 %.fca.6.9.load1033, 6, 9
  %.fca.7.0.load1036 = load i32, ptr %.fca.7.0.gep495, align 4
  %.fca.7.0.insert1037 = insertvalue [10 x [10 x i32]] %.fca.6.9.insert1034, i32 %.fca.7.0.load1036, 7, 0
  %.fca.7.1.load1039 = load i32, ptr %.fca.7.1.gep496, align 4
  %.fca.7.1.insert1040 = insertvalue [10 x [10 x i32]] %.fca.7.0.insert1037, i32 %.fca.7.1.load1039, 7, 1
  %.fca.7.2.load1042 = load i32, ptr %.fca.7.2.gep497, align 4
  %.fca.7.2.insert1043 = insertvalue [10 x [10 x i32]] %.fca.7.1.insert1040, i32 %.fca.7.2.load1042, 7, 2
  %.fca.7.3.load1045 = load i32, ptr %.fca.7.3.gep498, align 4
  %.fca.7.3.insert1046 = insertvalue [10 x [10 x i32]] %.fca.7.2.insert1043, i32 %.fca.7.3.load1045, 7, 3
  %.fca.7.4.load1048 = load i32, ptr %.fca.7.4.gep499, align 4
  %.fca.7.4.insert1049 = insertvalue [10 x [10 x i32]] %.fca.7.3.insert1046, i32 %.fca.7.4.load1048, 7, 4
  %.fca.7.5.load1051 = load i32, ptr %.fca.7.5.gep500, align 4
  %.fca.7.5.insert1052 = insertvalue [10 x [10 x i32]] %.fca.7.4.insert1049, i32 %.fca.7.5.load1051, 7, 5
  %.fca.7.6.load1054 = load i32, ptr %.fca.7.6.gep501, align 4
  %.fca.7.6.insert1055 = insertvalue [10 x [10 x i32]] %.fca.7.5.insert1052, i32 %.fca.7.6.load1054, 7, 6
  %.fca.7.7.load1057 = load i32, ptr %.fca.7.7.gep502, align 4
  %.fca.7.7.insert1058 = insertvalue [10 x [10 x i32]] %.fca.7.6.insert1055, i32 %.fca.7.7.load1057, 7, 7
  %.fca.7.8.load1060 = load i32, ptr %.fca.7.8.gep503, align 4
  %.fca.7.8.insert1061 = insertvalue [10 x [10 x i32]] %.fca.7.7.insert1058, i32 %.fca.7.8.load1060, 7, 8
  %.fca.7.9.load1063 = load i32, ptr %.fca.7.9.gep504, align 4
  %.fca.7.9.insert1064 = insertvalue [10 x [10 x i32]] %.fca.7.8.insert1061, i32 %.fca.7.9.load1063, 7, 9
  %.fca.8.0.load1066 = load i32, ptr %.fca.8.0.gep505, align 4
  %.fca.8.0.insert1067 = insertvalue [10 x [10 x i32]] %.fca.7.9.insert1064, i32 %.fca.8.0.load1066, 8, 0
  %.fca.8.1.load1069 = load i32, ptr %.fca.8.1.gep506, align 4
  %.fca.8.1.insert1070 = insertvalue [10 x [10 x i32]] %.fca.8.0.insert1067, i32 %.fca.8.1.load1069, 8, 1
  %.fca.8.2.load1072 = load i32, ptr %.fca.8.2.gep507, align 4
  %.fca.8.2.insert1073 = insertvalue [10 x [10 x i32]] %.fca.8.1.insert1070, i32 %.fca.8.2.load1072, 8, 2
  %.fca.8.3.load1075 = load i32, ptr %.fca.8.3.gep508, align 4
  %.fca.8.3.insert1076 = insertvalue [10 x [10 x i32]] %.fca.8.2.insert1073, i32 %.fca.8.3.load1075, 8, 3
  %.fca.8.4.load1078 = load i32, ptr %.fca.8.4.gep509, align 4
  %.fca.8.4.insert1079 = insertvalue [10 x [10 x i32]] %.fca.8.3.insert1076, i32 %.fca.8.4.load1078, 8, 4
  %.fca.8.5.load1081 = load i32, ptr %.fca.8.5.gep510, align 4
  %.fca.8.5.insert1082 = insertvalue [10 x [10 x i32]] %.fca.8.4.insert1079, i32 %.fca.8.5.load1081, 8, 5
  %.fca.8.6.load1084 = load i32, ptr %.fca.8.6.gep511, align 4
  %.fca.8.6.insert1085 = insertvalue [10 x [10 x i32]] %.fca.8.5.insert1082, i32 %.fca.8.6.load1084, 8, 6
  %.fca.8.7.load1087 = load i32, ptr %.fca.8.7.gep512, align 4
  %.fca.8.7.insert1088 = insertvalue [10 x [10 x i32]] %.fca.8.6.insert1085, i32 %.fca.8.7.load1087, 8, 7
  %.fca.8.8.load1090 = load i32, ptr %.fca.8.8.gep513, align 4
  %.fca.8.8.insert1091 = insertvalue [10 x [10 x i32]] %.fca.8.7.insert1088, i32 %.fca.8.8.load1090, 8, 8
  %.fca.8.9.load1093 = load i32, ptr %.fca.8.9.gep514, align 4
  %.fca.8.9.insert1094 = insertvalue [10 x [10 x i32]] %.fca.8.8.insert1091, i32 %.fca.8.9.load1093, 8, 9
  %.fca.9.0.load1096 = load i32, ptr %.fca.9.0.gep515, align 4
  %.fca.9.0.insert1097 = insertvalue [10 x [10 x i32]] %.fca.8.9.insert1094, i32 %.fca.9.0.load1096, 9, 0
  %.fca.9.1.load1099 = load i32, ptr %.fca.9.1.gep516, align 4
  %.fca.9.1.insert1100 = insertvalue [10 x [10 x i32]] %.fca.9.0.insert1097, i32 %.fca.9.1.load1099, 9, 1
  %.fca.9.2.load1102 = load i32, ptr %.fca.9.2.gep517, align 4
  %.fca.9.2.insert1103 = insertvalue [10 x [10 x i32]] %.fca.9.1.insert1100, i32 %.fca.9.2.load1102, 9, 2
  %.fca.9.3.load1105 = load i32, ptr %.fca.9.3.gep518, align 4
  %.fca.9.3.insert1106 = insertvalue [10 x [10 x i32]] %.fca.9.2.insert1103, i32 %.fca.9.3.load1105, 9, 3
  %.fca.9.4.load1108 = load i32, ptr %.fca.9.4.gep519, align 4
  %.fca.9.4.insert1109 = insertvalue [10 x [10 x i32]] %.fca.9.3.insert1106, i32 %.fca.9.4.load1108, 9, 4
  %.fca.9.5.load1111 = load i32, ptr %.fca.9.5.gep520, align 4
  %.fca.9.5.insert1112 = insertvalue [10 x [10 x i32]] %.fca.9.4.insert1109, i32 %.fca.9.5.load1111, 9, 5
  %.fca.9.6.load1114 = load i32, ptr %.fca.9.6.gep521, align 4
  %.fca.9.6.insert1115 = insertvalue [10 x [10 x i32]] %.fca.9.5.insert1112, i32 %.fca.9.6.load1114, 9, 6
  %.fca.9.7.load1117 = load i32, ptr %.fca.9.7.gep522, align 4
  %.fca.9.7.insert1118 = insertvalue [10 x [10 x i32]] %.fca.9.6.insert1115, i32 %.fca.9.7.load1117, 9, 7
  %.fca.9.8.load1120 = load i32, ptr %.fca.9.8.gep523, align 4
  %.fca.9.8.insert1121 = insertvalue [10 x [10 x i32]] %.fca.9.7.insert1118, i32 %.fca.9.8.load1120, 9, 8
  %.fca.9.9.load1123 = load i32, ptr %.fca.9.9.gep524, align 4
  %.fca.9.9.insert1124 = insertvalue [10 x [10 x i32]] %.fca.9.8.insert1121, i32 %.fca.9.9.load1123, 9, 9
  br label %for.head.L74.preheader

for.head.L74.preheader:                           ; preds = %for.head.L73.preheader, %for.tail.L73
  %indvars.iv1136 = phi i64 [ 0, %for.head.L73.preheader ], [ %indvars.iv.next1137, %for.tail.L73 ]
  %21 = trunc i64 %indvars.iv1136 to i32
  br label %for.body.L74

for.body.L74:                                     ; preds = %for.head.L74.preheader, %for.tail.L74
  %indvars.iv1132 = phi i64 [ 0, %for.head.L74.preheader ], [ %indvars.iv.next1133, %for.tail.L74 ]
  %22 = trunc i64 %indvars.iv1132 to i32
  %23 = tail call fastcc i32 @_f__void__int__countLiveNeighborCell__intarrayarray_int_int([10 x [10 x i32]] %.fca.9.9.insert1124, i32 %21, i32 %22) #5
  %24 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i64 %indvars.iv1136, i64 %indvars.iv1132
  %25 = load i32, ptr %24, align 4
  %26 = icmp eq i32 %25, 1
  %27 = and i32 %23, -2
  %lor_phi = icmp eq i32 %27, 2
  %land_phi = select i1 %26, i1 %lor_phi, i1 false
  br i1 %land_phi, label %if.then.L76, label %if.else.L76

if.then.L76:                                      ; preds = %for.body.L74
  %28 = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 %indvars.iv1136, i64 %indvars.iv1132
  store i32 1, ptr %28, align 4
  br label %for.tail.L74

if.else.L76:                                      ; preds = %for.body.L74
  %29 = icmp eq i32 %25, 0
  %30 = icmp eq i32 %23, 3
  %land_phi3 = select i1 %29, i1 %30, i1 false
  %31 = getelementptr inbounds [10 x [10 x i32]], ptr %b, i64 0, i64 %indvars.iv1136, i64 %indvars.iv1132
  br i1 %land_phi3, label %if.then.L78, label %if.else.L78

if.then.L78:                                      ; preds = %if.else.L76
  store i32 1, ptr %31, align 4
  br label %for.tail.L74

if.else.L78:                                      ; preds = %if.else.L76
  store i32 0, ptr %31, align 4
  br label %for.tail.L74

for.tail.L74:                                     ; preds = %if.then.L76, %if.else.L78, %if.then.L78
  %indvars.iv.next1133 = add nuw nsw i64 %indvars.iv1132, 1
  %exitcond1135.not = icmp eq i64 %indvars.iv.next1133, 10
  br i1 %exitcond1135.not, label %for.tail.L73, label %for.body.L74

for.tail.L73:                                     ; preds = %for.tail.L74
  %indvars.iv.next1137 = add nuw nsw i64 %indvars.iv1136, 1
  %exitcond1139.not = icmp eq i64 %indvars.iv.next1137, 10
  br i1 %exitcond1139.not, label %for.exit.L73, label %for.head.L74.preheader

for.exit.L73:                                     ; preds = %for.tail.L73
  %.fca.0.0.load = load i32, ptr %b, align 4
  %.fca.0.0.insert = insertvalue [10 x [10 x i32]] poison, i32 %.fca.0.0.load, 0, 0
  %.fca.0.1.load = load i32, ptr %.fca.0.1.gep, align 4
  %.fca.0.1.insert = insertvalue [10 x [10 x i32]] %.fca.0.0.insert, i32 %.fca.0.1.load, 0, 1
  %.fca.0.2.load = load i32, ptr %.fca.0.2.gep, align 4
  %.fca.0.2.insert = insertvalue [10 x [10 x i32]] %.fca.0.1.insert, i32 %.fca.0.2.load, 0, 2
  %.fca.0.3.load = load i32, ptr %.fca.0.3.gep, align 4
  %.fca.0.3.insert = insertvalue [10 x [10 x i32]] %.fca.0.2.insert, i32 %.fca.0.3.load, 0, 3
  %.fca.0.4.load = load i32, ptr %.fca.0.4.gep, align 4
  %.fca.0.4.insert = insertvalue [10 x [10 x i32]] %.fca.0.3.insert, i32 %.fca.0.4.load, 0, 4
  %.fca.0.5.load = load i32, ptr %.fca.0.5.gep, align 4
  %.fca.0.5.insert = insertvalue [10 x [10 x i32]] %.fca.0.4.insert, i32 %.fca.0.5.load, 0, 5
  %.fca.0.6.load = load i32, ptr %.fca.0.6.gep, align 4
  %.fca.0.6.insert = insertvalue [10 x [10 x i32]] %.fca.0.5.insert, i32 %.fca.0.6.load, 0, 6
  %.fca.0.7.load = load i32, ptr %.fca.0.7.gep, align 4
  %.fca.0.7.insert = insertvalue [10 x [10 x i32]] %.fca.0.6.insert, i32 %.fca.0.7.load, 0, 7
  %.fca.0.8.load = load i32, ptr %.fca.0.8.gep, align 4
  %.fca.0.8.insert = insertvalue [10 x [10 x i32]] %.fca.0.7.insert, i32 %.fca.0.8.load, 0, 8
  %.fca.0.9.load = load i32, ptr %.fca.0.9.gep, align 4
  %.fca.0.9.insert = insertvalue [10 x [10 x i32]] %.fca.0.8.insert, i32 %.fca.0.9.load, 0, 9
  %.fca.1.0.load = load i32, ptr %.fca.1.0.gep, align 4
  %.fca.1.0.insert = insertvalue [10 x [10 x i32]] %.fca.0.9.insert, i32 %.fca.1.0.load, 1, 0
  %.fca.1.1.load = load i32, ptr %.fca.1.1.gep, align 4
  %.fca.1.1.insert = insertvalue [10 x [10 x i32]] %.fca.1.0.insert, i32 %.fca.1.1.load, 1, 1
  %.fca.1.2.load = load i32, ptr %.fca.1.2.gep, align 4
  %.fca.1.2.insert = insertvalue [10 x [10 x i32]] %.fca.1.1.insert, i32 %.fca.1.2.load, 1, 2
  %.fca.1.3.load = load i32, ptr %.fca.1.3.gep, align 4
  %.fca.1.3.insert = insertvalue [10 x [10 x i32]] %.fca.1.2.insert, i32 %.fca.1.3.load, 1, 3
  %.fca.1.4.load = load i32, ptr %.fca.1.4.gep, align 4
  %.fca.1.4.insert = insertvalue [10 x [10 x i32]] %.fca.1.3.insert, i32 %.fca.1.4.load, 1, 4
  %.fca.1.5.load = load i32, ptr %.fca.1.5.gep, align 4
  %.fca.1.5.insert = insertvalue [10 x [10 x i32]] %.fca.1.4.insert, i32 %.fca.1.5.load, 1, 5
  %.fca.1.6.load = load i32, ptr %.fca.1.6.gep, align 4
  %.fca.1.6.insert = insertvalue [10 x [10 x i32]] %.fca.1.5.insert, i32 %.fca.1.6.load, 1, 6
  %.fca.1.7.load = load i32, ptr %.fca.1.7.gep, align 4
  %.fca.1.7.insert = insertvalue [10 x [10 x i32]] %.fca.1.6.insert, i32 %.fca.1.7.load, 1, 7
  %.fca.1.8.load = load i32, ptr %.fca.1.8.gep, align 4
  %.fca.1.8.insert = insertvalue [10 x [10 x i32]] %.fca.1.7.insert, i32 %.fca.1.8.load, 1, 8
  %.fca.1.9.load = load i32, ptr %.fca.1.9.gep, align 4
  %.fca.1.9.insert = insertvalue [10 x [10 x i32]] %.fca.1.8.insert, i32 %.fca.1.9.load, 1, 9
  %.fca.2.0.load = load i32, ptr %.fca.2.0.gep, align 4
  %.fca.2.0.insert = insertvalue [10 x [10 x i32]] %.fca.1.9.insert, i32 %.fca.2.0.load, 2, 0
  %.fca.2.1.load = load i32, ptr %.fca.2.1.gep, align 4
  %.fca.2.1.insert = insertvalue [10 x [10 x i32]] %.fca.2.0.insert, i32 %.fca.2.1.load, 2, 1
  %.fca.2.2.load = load i32, ptr %.fca.2.2.gep, align 4
  %.fca.2.2.insert = insertvalue [10 x [10 x i32]] %.fca.2.1.insert, i32 %.fca.2.2.load, 2, 2
  %.fca.2.3.load = load i32, ptr %.fca.2.3.gep, align 4
  %.fca.2.3.insert = insertvalue [10 x [10 x i32]] %.fca.2.2.insert, i32 %.fca.2.3.load, 2, 3
  %.fca.2.4.load = load i32, ptr %.fca.2.4.gep, align 4
  %.fca.2.4.insert = insertvalue [10 x [10 x i32]] %.fca.2.3.insert, i32 %.fca.2.4.load, 2, 4
  %.fca.2.5.load = load i32, ptr %.fca.2.5.gep, align 4
  %.fca.2.5.insert = insertvalue [10 x [10 x i32]] %.fca.2.4.insert, i32 %.fca.2.5.load, 2, 5
  %.fca.2.6.load = load i32, ptr %.fca.2.6.gep, align 4
  %.fca.2.6.insert = insertvalue [10 x [10 x i32]] %.fca.2.5.insert, i32 %.fca.2.6.load, 2, 6
  %.fca.2.7.load = load i32, ptr %.fca.2.7.gep, align 4
  %.fca.2.7.insert = insertvalue [10 x [10 x i32]] %.fca.2.6.insert, i32 %.fca.2.7.load, 2, 7
  %.fca.2.8.load = load i32, ptr %.fca.2.8.gep, align 4
  %.fca.2.8.insert = insertvalue [10 x [10 x i32]] %.fca.2.7.insert, i32 %.fca.2.8.load, 2, 8
  %.fca.2.9.load = load i32, ptr %.fca.2.9.gep, align 4
  %.fca.2.9.insert = insertvalue [10 x [10 x i32]] %.fca.2.8.insert, i32 %.fca.2.9.load, 2, 9
  %.fca.3.0.load = load i32, ptr %.fca.3.0.gep, align 4
  %.fca.3.0.insert = insertvalue [10 x [10 x i32]] %.fca.2.9.insert, i32 %.fca.3.0.load, 3, 0
  %.fca.3.1.load = load i32, ptr %.fca.3.1.gep, align 4
  %.fca.3.1.insert = insertvalue [10 x [10 x i32]] %.fca.3.0.insert, i32 %.fca.3.1.load, 3, 1
  %.fca.3.2.load = load i32, ptr %.fca.3.2.gep, align 4
  %.fca.3.2.insert = insertvalue [10 x [10 x i32]] %.fca.3.1.insert, i32 %.fca.3.2.load, 3, 2
  %.fca.3.3.load = load i32, ptr %.fca.3.3.gep, align 4
  %.fca.3.3.insert = insertvalue [10 x [10 x i32]] %.fca.3.2.insert, i32 %.fca.3.3.load, 3, 3
  %.fca.3.4.load = load i32, ptr %.fca.3.4.gep, align 4
  %.fca.3.4.insert = insertvalue [10 x [10 x i32]] %.fca.3.3.insert, i32 %.fca.3.4.load, 3, 4
  %.fca.3.5.load = load i32, ptr %.fca.3.5.gep, align 4
  %.fca.3.5.insert = insertvalue [10 x [10 x i32]] %.fca.3.4.insert, i32 %.fca.3.5.load, 3, 5
  %.fca.3.6.load = load i32, ptr %.fca.3.6.gep, align 4
  %.fca.3.6.insert = insertvalue [10 x [10 x i32]] %.fca.3.5.insert, i32 %.fca.3.6.load, 3, 6
  %.fca.3.7.load = load i32, ptr %.fca.3.7.gep, align 4
  %.fca.3.7.insert = insertvalue [10 x [10 x i32]] %.fca.3.6.insert, i32 %.fca.3.7.load, 3, 7
  %.fca.3.8.load = load i32, ptr %.fca.3.8.gep, align 4
  %.fca.3.8.insert = insertvalue [10 x [10 x i32]] %.fca.3.7.insert, i32 %.fca.3.8.load, 3, 8
  %.fca.3.9.load = load i32, ptr %.fca.3.9.gep, align 4
  %.fca.3.9.insert = insertvalue [10 x [10 x i32]] %.fca.3.8.insert, i32 %.fca.3.9.load, 3, 9
  %.fca.4.0.load = load i32, ptr %.fca.4.0.gep, align 4
  %.fca.4.0.insert = insertvalue [10 x [10 x i32]] %.fca.3.9.insert, i32 %.fca.4.0.load, 4, 0
  %.fca.4.1.load = load i32, ptr %.fca.4.1.gep, align 4
  %.fca.4.1.insert = insertvalue [10 x [10 x i32]] %.fca.4.0.insert, i32 %.fca.4.1.load, 4, 1
  %.fca.4.2.load = load i32, ptr %.fca.4.2.gep, align 4
  %.fca.4.2.insert = insertvalue [10 x [10 x i32]] %.fca.4.1.insert, i32 %.fca.4.2.load, 4, 2
  %.fca.4.3.load = load i32, ptr %.fca.4.3.gep, align 4
  %.fca.4.3.insert = insertvalue [10 x [10 x i32]] %.fca.4.2.insert, i32 %.fca.4.3.load, 4, 3
  %.fca.4.4.load = load i32, ptr %.fca.4.4.gep, align 4
  %.fca.4.4.insert = insertvalue [10 x [10 x i32]] %.fca.4.3.insert, i32 %.fca.4.4.load, 4, 4
  %.fca.4.5.load = load i32, ptr %.fca.4.5.gep, align 4
  %.fca.4.5.insert = insertvalue [10 x [10 x i32]] %.fca.4.4.insert, i32 %.fca.4.5.load, 4, 5
  %.fca.4.6.load = load i32, ptr %.fca.4.6.gep, align 4
  %.fca.4.6.insert = insertvalue [10 x [10 x i32]] %.fca.4.5.insert, i32 %.fca.4.6.load, 4, 6
  %.fca.4.7.load = load i32, ptr %.fca.4.7.gep, align 4
  %.fca.4.7.insert = insertvalue [10 x [10 x i32]] %.fca.4.6.insert, i32 %.fca.4.7.load, 4, 7
  %.fca.4.8.load = load i32, ptr %.fca.4.8.gep, align 4
  %.fca.4.8.insert = insertvalue [10 x [10 x i32]] %.fca.4.7.insert, i32 %.fca.4.8.load, 4, 8
  %.fca.4.9.load = load i32, ptr %.fca.4.9.gep, align 4
  %.fca.4.9.insert = insertvalue [10 x [10 x i32]] %.fca.4.8.insert, i32 %.fca.4.9.load, 4, 9
  %.fca.5.0.load = load i32, ptr %.fca.5.0.gep, align 4
  %.fca.5.0.insert = insertvalue [10 x [10 x i32]] %.fca.4.9.insert, i32 %.fca.5.0.load, 5, 0
  %.fca.5.1.load = load i32, ptr %.fca.5.1.gep, align 4
  %.fca.5.1.insert = insertvalue [10 x [10 x i32]] %.fca.5.0.insert, i32 %.fca.5.1.load, 5, 1
  %.fca.5.2.load = load i32, ptr %.fca.5.2.gep, align 4
  %.fca.5.2.insert = insertvalue [10 x [10 x i32]] %.fca.5.1.insert, i32 %.fca.5.2.load, 5, 2
  %.fca.5.3.load = load i32, ptr %.fca.5.3.gep, align 4
  %.fca.5.3.insert = insertvalue [10 x [10 x i32]] %.fca.5.2.insert, i32 %.fca.5.3.load, 5, 3
  %.fca.5.4.load = load i32, ptr %.fca.5.4.gep, align 4
  %.fca.5.4.insert = insertvalue [10 x [10 x i32]] %.fca.5.3.insert, i32 %.fca.5.4.load, 5, 4
  %.fca.5.5.load = load i32, ptr %.fca.5.5.gep, align 4
  %.fca.5.5.insert = insertvalue [10 x [10 x i32]] %.fca.5.4.insert, i32 %.fca.5.5.load, 5, 5
  %.fca.5.6.load = load i32, ptr %.fca.5.6.gep, align 4
  %.fca.5.6.insert = insertvalue [10 x [10 x i32]] %.fca.5.5.insert, i32 %.fca.5.6.load, 5, 6
  %.fca.5.7.load = load i32, ptr %.fca.5.7.gep, align 4
  %.fca.5.7.insert = insertvalue [10 x [10 x i32]] %.fca.5.6.insert, i32 %.fca.5.7.load, 5, 7
  %.fca.5.8.load = load i32, ptr %.fca.5.8.gep, align 4
  %.fca.5.8.insert = insertvalue [10 x [10 x i32]] %.fca.5.7.insert, i32 %.fca.5.8.load, 5, 8
  %.fca.5.9.load = load i32, ptr %.fca.5.9.gep, align 4
  %.fca.5.9.insert = insertvalue [10 x [10 x i32]] %.fca.5.8.insert, i32 %.fca.5.9.load, 5, 9
  %.fca.6.0.load = load i32, ptr %.fca.6.0.gep, align 4
  %.fca.6.0.insert = insertvalue [10 x [10 x i32]] %.fca.5.9.insert, i32 %.fca.6.0.load, 6, 0
  %.fca.6.1.load = load i32, ptr %.fca.6.1.gep, align 4
  %.fca.6.1.insert = insertvalue [10 x [10 x i32]] %.fca.6.0.insert, i32 %.fca.6.1.load, 6, 1
  %.fca.6.2.load = load i32, ptr %.fca.6.2.gep, align 4
  %.fca.6.2.insert = insertvalue [10 x [10 x i32]] %.fca.6.1.insert, i32 %.fca.6.2.load, 6, 2
  %.fca.6.3.load = load i32, ptr %.fca.6.3.gep, align 4
  %.fca.6.3.insert = insertvalue [10 x [10 x i32]] %.fca.6.2.insert, i32 %.fca.6.3.load, 6, 3
  %.fca.6.4.load = load i32, ptr %.fca.6.4.gep, align 4
  %.fca.6.4.insert = insertvalue [10 x [10 x i32]] %.fca.6.3.insert, i32 %.fca.6.4.load, 6, 4
  %.fca.6.5.load = load i32, ptr %.fca.6.5.gep, align 4
  %.fca.6.5.insert = insertvalue [10 x [10 x i32]] %.fca.6.4.insert, i32 %.fca.6.5.load, 6, 5
  %.fca.6.6.load = load i32, ptr %.fca.6.6.gep, align 4
  %.fca.6.6.insert = insertvalue [10 x [10 x i32]] %.fca.6.5.insert, i32 %.fca.6.6.load, 6, 6
  %.fca.6.7.load = load i32, ptr %.fca.6.7.gep, align 4
  %.fca.6.7.insert = insertvalue [10 x [10 x i32]] %.fca.6.6.insert, i32 %.fca.6.7.load, 6, 7
  %.fca.6.8.load = load i32, ptr %.fca.6.8.gep, align 4
  %.fca.6.8.insert = insertvalue [10 x [10 x i32]] %.fca.6.7.insert, i32 %.fca.6.8.load, 6, 8
  %.fca.6.9.load = load i32, ptr %.fca.6.9.gep, align 4
  %.fca.6.9.insert = insertvalue [10 x [10 x i32]] %.fca.6.8.insert, i32 %.fca.6.9.load, 6, 9
  %.fca.7.0.load = load i32, ptr %.fca.7.0.gep, align 4
  %.fca.7.0.insert = insertvalue [10 x [10 x i32]] %.fca.6.9.insert, i32 %.fca.7.0.load, 7, 0
  %.fca.7.1.load = load i32, ptr %.fca.7.1.gep, align 4
  %.fca.7.1.insert = insertvalue [10 x [10 x i32]] %.fca.7.0.insert, i32 %.fca.7.1.load, 7, 1
  %.fca.7.2.load = load i32, ptr %.fca.7.2.gep, align 4
  %.fca.7.2.insert = insertvalue [10 x [10 x i32]] %.fca.7.1.insert, i32 %.fca.7.2.load, 7, 2
  %.fca.7.3.load = load i32, ptr %.fca.7.3.gep, align 4
  %.fca.7.3.insert = insertvalue [10 x [10 x i32]] %.fca.7.2.insert, i32 %.fca.7.3.load, 7, 3
  %.fca.7.4.load = load i32, ptr %.fca.7.4.gep, align 4
  %.fca.7.4.insert = insertvalue [10 x [10 x i32]] %.fca.7.3.insert, i32 %.fca.7.4.load, 7, 4
  %.fca.7.5.load = load i32, ptr %.fca.7.5.gep, align 4
  %.fca.7.5.insert = insertvalue [10 x [10 x i32]] %.fca.7.4.insert, i32 %.fca.7.5.load, 7, 5
  %.fca.7.6.load = load i32, ptr %.fca.7.6.gep, align 4
  %.fca.7.6.insert = insertvalue [10 x [10 x i32]] %.fca.7.5.insert, i32 %.fca.7.6.load, 7, 6
  %.fca.7.7.load = load i32, ptr %.fca.7.7.gep, align 4
  %.fca.7.7.insert = insertvalue [10 x [10 x i32]] %.fca.7.6.insert, i32 %.fca.7.7.load, 7, 7
  %.fca.7.8.load = load i32, ptr %.fca.7.8.gep, align 4
  %.fca.7.8.insert = insertvalue [10 x [10 x i32]] %.fca.7.7.insert, i32 %.fca.7.8.load, 7, 8
  %.fca.7.9.load = load i32, ptr %.fca.7.9.gep, align 4
  %.fca.7.9.insert = insertvalue [10 x [10 x i32]] %.fca.7.8.insert, i32 %.fca.7.9.load, 7, 9
  %.fca.8.0.load = load i32, ptr %.fca.8.0.gep, align 4
  %.fca.8.0.insert = insertvalue [10 x [10 x i32]] %.fca.7.9.insert, i32 %.fca.8.0.load, 8, 0
  %.fca.8.1.load = load i32, ptr %.fca.8.1.gep, align 4
  %.fca.8.1.insert = insertvalue [10 x [10 x i32]] %.fca.8.0.insert, i32 %.fca.8.1.load, 8, 1
  %.fca.8.2.load = load i32, ptr %.fca.8.2.gep, align 4
  %.fca.8.2.insert = insertvalue [10 x [10 x i32]] %.fca.8.1.insert, i32 %.fca.8.2.load, 8, 2
  %.fca.8.3.load = load i32, ptr %.fca.8.3.gep, align 4
  %.fca.8.3.insert = insertvalue [10 x [10 x i32]] %.fca.8.2.insert, i32 %.fca.8.3.load, 8, 3
  %.fca.8.4.load = load i32, ptr %.fca.8.4.gep, align 4
  %.fca.8.4.insert = insertvalue [10 x [10 x i32]] %.fca.8.3.insert, i32 %.fca.8.4.load, 8, 4
  %.fca.8.5.load = load i32, ptr %.fca.8.5.gep, align 4
  %.fca.8.5.insert = insertvalue [10 x [10 x i32]] %.fca.8.4.insert, i32 %.fca.8.5.load, 8, 5
  %.fca.8.6.load = load i32, ptr %.fca.8.6.gep, align 4
  %.fca.8.6.insert = insertvalue [10 x [10 x i32]] %.fca.8.5.insert, i32 %.fca.8.6.load, 8, 6
  %.fca.8.7.load = load i32, ptr %.fca.8.7.gep, align 4
  %.fca.8.7.insert = insertvalue [10 x [10 x i32]] %.fca.8.6.insert, i32 %.fca.8.7.load, 8, 7
  %.fca.8.8.load = load i32, ptr %.fca.8.8.gep, align 4
  %.fca.8.8.insert = insertvalue [10 x [10 x i32]] %.fca.8.7.insert, i32 %.fca.8.8.load, 8, 8
  %.fca.8.9.load = load i32, ptr %.fca.8.9.gep, align 4
  %.fca.8.9.insert = insertvalue [10 x [10 x i32]] %.fca.8.8.insert, i32 %.fca.8.9.load, 8, 9
  %.fca.9.0.load = load i32, ptr %.fca.9.0.gep, align 4
  %.fca.9.0.insert = insertvalue [10 x [10 x i32]] %.fca.8.9.insert, i32 %.fca.9.0.load, 9, 0
  %.fca.9.1.load = load i32, ptr %.fca.9.1.gep, align 4
  %.fca.9.1.insert = insertvalue [10 x [10 x i32]] %.fca.9.0.insert, i32 %.fca.9.1.load, 9, 1
  %.fca.9.2.load = load i32, ptr %.fca.9.2.gep, align 4
  %.fca.9.2.insert = insertvalue [10 x [10 x i32]] %.fca.9.1.insert, i32 %.fca.9.2.load, 9, 2
  %.fca.9.3.load = load i32, ptr %.fca.9.3.gep, align 4
  %.fca.9.3.insert = insertvalue [10 x [10 x i32]] %.fca.9.2.insert, i32 %.fca.9.3.load, 9, 3
  %.fca.9.4.load = load i32, ptr %.fca.9.4.gep, align 4
  %.fca.9.4.insert = insertvalue [10 x [10 x i32]] %.fca.9.3.insert, i32 %.fca.9.4.load, 9, 4
  %.fca.9.5.load = load i32, ptr %.fca.9.5.gep, align 4
  %.fca.9.5.insert = insertvalue [10 x [10 x i32]] %.fca.9.4.insert, i32 %.fca.9.5.load, 9, 5
  %.fca.9.6.load = load i32, ptr %.fca.9.6.gep, align 4
  %.fca.9.6.insert = insertvalue [10 x [10 x i32]] %.fca.9.5.insert, i32 %.fca.9.6.load, 9, 6
  %.fca.9.7.load = load i32, ptr %.fca.9.7.gep, align 4
  %.fca.9.7.insert = insertvalue [10 x [10 x i32]] %.fca.9.6.insert, i32 %.fca.9.7.load, 9, 7
  %.fca.9.8.load = load i32, ptr %.fca.9.8.gep, align 4
  %.fca.9.8.insert = insertvalue [10 x [10 x i32]] %.fca.9.7.insert, i32 %.fca.9.8.load, 9, 8
  %.fca.9.9.load = load i32, ptr %.fca.9.9.gep, align 4
  %.fca.9.9.insert = insertvalue [10 x [10 x i32]] %.fca.9.8.insert, i32 %.fca.9.9.load, 9, 9
  tail call fastcc void @_p__void__void__printGeneration__string_intarrayarray(ptr nonnull @anon.string.1, [10 x [10 x i32]] %.fca.9.9.insert) #5
  %.fca.0.0.load126 = load i32, ptr %b, align 4
  %.fca.0.1.load129 = load i32, ptr %.fca.0.1.gep, align 4
  %.fca.0.2.load132 = load i32, ptr %.fca.0.2.gep, align 4
  %.fca.0.3.load135 = load i32, ptr %.fca.0.3.gep, align 4
  %.fca.0.4.load138 = load i32, ptr %.fca.0.4.gep, align 4
  %.fca.0.5.load141 = load i32, ptr %.fca.0.5.gep, align 4
  %.fca.0.6.load144 = load i32, ptr %.fca.0.6.gep, align 4
  %.fca.0.7.load147 = load i32, ptr %.fca.0.7.gep, align 4
  %.fca.0.8.load150 = load i32, ptr %.fca.0.8.gep, align 4
  %.fca.0.9.load153 = load i32, ptr %.fca.0.9.gep, align 4
  %.fca.1.0.load156 = load i32, ptr %.fca.1.0.gep, align 4
  %.fca.1.1.load159 = load i32, ptr %.fca.1.1.gep, align 4
  %.fca.1.2.load162 = load i32, ptr %.fca.1.2.gep, align 4
  %.fca.1.3.load165 = load i32, ptr %.fca.1.3.gep, align 4
  %.fca.1.4.load168 = load i32, ptr %.fca.1.4.gep, align 4
  %.fca.1.5.load171 = load i32, ptr %.fca.1.5.gep, align 4
  %.fca.1.6.load174 = load i32, ptr %.fca.1.6.gep, align 4
  %.fca.1.7.load177 = load i32, ptr %.fca.1.7.gep, align 4
  %.fca.1.8.load180 = load i32, ptr %.fca.1.8.gep, align 4
  %.fca.1.9.load183 = load i32, ptr %.fca.1.9.gep, align 4
  %.fca.2.0.load186 = load i32, ptr %.fca.2.0.gep, align 4
  %.fca.2.1.load189 = load i32, ptr %.fca.2.1.gep, align 4
  %.fca.2.2.load192 = load i32, ptr %.fca.2.2.gep, align 4
  %.fca.2.3.load195 = load i32, ptr %.fca.2.3.gep, align 4
  %.fca.2.4.load198 = load i32, ptr %.fca.2.4.gep, align 4
  %.fca.2.5.load201 = load i32, ptr %.fca.2.5.gep, align 4
  %.fca.2.6.load204 = load i32, ptr %.fca.2.6.gep, align 4
  %.fca.2.7.load207 = load i32, ptr %.fca.2.7.gep, align 4
  %.fca.2.8.load210 = load i32, ptr %.fca.2.8.gep, align 4
  %.fca.2.9.load213 = load i32, ptr %.fca.2.9.gep, align 4
  %.fca.3.0.load216 = load i32, ptr %.fca.3.0.gep, align 4
  %.fca.3.1.load219 = load i32, ptr %.fca.3.1.gep, align 4
  %.fca.3.2.load222 = load i32, ptr %.fca.3.2.gep, align 4
  %.fca.3.3.load225 = load i32, ptr %.fca.3.3.gep, align 4
  %.fca.3.4.load228 = load i32, ptr %.fca.3.4.gep, align 4
  %.fca.3.5.load231 = load i32, ptr %.fca.3.5.gep, align 4
  %.fca.3.6.load234 = load i32, ptr %.fca.3.6.gep, align 4
  %.fca.3.7.load237 = load i32, ptr %.fca.3.7.gep, align 4
  %.fca.3.8.load240 = load i32, ptr %.fca.3.8.gep, align 4
  %.fca.3.9.load243 = load i32, ptr %.fca.3.9.gep, align 4
  %.fca.4.0.load246 = load i32, ptr %.fca.4.0.gep, align 4
  %.fca.4.1.load249 = load i32, ptr %.fca.4.1.gep, align 4
  %.fca.4.2.load252 = load i32, ptr %.fca.4.2.gep, align 4
  %.fca.4.3.load255 = load i32, ptr %.fca.4.3.gep, align 4
  %.fca.4.4.load258 = load i32, ptr %.fca.4.4.gep, align 4
  %.fca.4.5.load261 = load i32, ptr %.fca.4.5.gep, align 4
  %.fca.4.6.load264 = load i32, ptr %.fca.4.6.gep, align 4
  %.fca.4.7.load267 = load i32, ptr %.fca.4.7.gep, align 4
  %.fca.4.8.load270 = load i32, ptr %.fca.4.8.gep, align 4
  %.fca.4.9.load273 = load i32, ptr %.fca.4.9.gep, align 4
  %.fca.5.0.load276 = load i32, ptr %.fca.5.0.gep, align 4
  %.fca.5.1.load279 = load i32, ptr %.fca.5.1.gep, align 4
  %.fca.5.2.load282 = load i32, ptr %.fca.5.2.gep, align 4
  %.fca.5.3.load285 = load i32, ptr %.fca.5.3.gep, align 4
  %.fca.5.4.load288 = load i32, ptr %.fca.5.4.gep, align 4
  %.fca.5.5.load291 = load i32, ptr %.fca.5.5.gep, align 4
  %.fca.5.6.load294 = load i32, ptr %.fca.5.6.gep, align 4
  %.fca.5.7.load297 = load i32, ptr %.fca.5.7.gep, align 4
  %.fca.5.8.load300 = load i32, ptr %.fca.5.8.gep, align 4
  %.fca.5.9.load303 = load i32, ptr %.fca.5.9.gep, align 4
  %.fca.6.0.load306 = load i32, ptr %.fca.6.0.gep, align 4
  %.fca.6.1.load309 = load i32, ptr %.fca.6.1.gep, align 4
  %.fca.6.2.load312 = load i32, ptr %.fca.6.2.gep, align 4
  %.fca.6.3.load315 = load i32, ptr %.fca.6.3.gep, align 4
  %.fca.6.4.load318 = load i32, ptr %.fca.6.4.gep, align 4
  %.fca.6.5.load321 = load i32, ptr %.fca.6.5.gep, align 4
  %.fca.6.6.load324 = load i32, ptr %.fca.6.6.gep, align 4
  %.fca.6.7.load327 = load i32, ptr %.fca.6.7.gep, align 4
  %.fca.6.8.load330 = load i32, ptr %.fca.6.8.gep, align 4
  %.fca.6.9.load333 = load i32, ptr %.fca.6.9.gep, align 4
  %.fca.7.0.load336 = load i32, ptr %.fca.7.0.gep, align 4
  %.fca.7.1.load339 = load i32, ptr %.fca.7.1.gep, align 4
  %.fca.7.2.load342 = load i32, ptr %.fca.7.2.gep, align 4
  %.fca.7.3.load345 = load i32, ptr %.fca.7.3.gep, align 4
  %.fca.7.4.load348 = load i32, ptr %.fca.7.4.gep, align 4
  %.fca.7.5.load351 = load i32, ptr %.fca.7.5.gep, align 4
  %.fca.7.6.load354 = load i32, ptr %.fca.7.6.gep, align 4
  %.fca.7.7.load357 = load i32, ptr %.fca.7.7.gep, align 4
  %.fca.7.8.load360 = load i32, ptr %.fca.7.8.gep, align 4
  %.fca.7.9.load363 = load i32, ptr %.fca.7.9.gep, align 4
  %.fca.8.0.load366 = load i32, ptr %.fca.8.0.gep, align 4
  %.fca.8.1.load369 = load i32, ptr %.fca.8.1.gep, align 4
  %.fca.8.2.load372 = load i32, ptr %.fca.8.2.gep, align 4
  %.fca.8.3.load375 = load i32, ptr %.fca.8.3.gep, align 4
  %.fca.8.4.load378 = load i32, ptr %.fca.8.4.gep, align 4
  %.fca.8.5.load381 = load i32, ptr %.fca.8.5.gep, align 4
  %.fca.8.6.load384 = load i32, ptr %.fca.8.6.gep, align 4
  %.fca.8.7.load387 = load i32, ptr %.fca.8.7.gep, align 4
  %.fca.8.8.load390 = load i32, ptr %.fca.8.8.gep, align 4
  %.fca.8.9.load393 = load i32, ptr %.fca.8.9.gep, align 4
  %.fca.9.0.load396 = load i32, ptr %.fca.9.0.gep, align 4
  %.fca.9.1.load399 = load i32, ptr %.fca.9.1.gep, align 4
  %.fca.9.2.load402 = load i32, ptr %.fca.9.2.gep, align 4
  %.fca.9.3.load405 = load i32, ptr %.fca.9.3.gep, align 4
  %.fca.9.4.load408 = load i32, ptr %.fca.9.4.gep, align 4
  %.fca.9.5.load411 = load i32, ptr %.fca.9.5.gep, align 4
  %.fca.9.6.load414 = load i32, ptr %.fca.9.6.gep, align 4
  %.fca.9.7.load417 = load i32, ptr %.fca.9.7.gep, align 4
  %.fca.9.8.load420 = load i32, ptr %.fca.9.8.gep, align 4
  %.fca.9.9.load423 = load i32, ptr %.fca.9.9.gep, align 4
  store i32 %.fca.0.0.load126, ptr %a, align 4
  store i32 %.fca.0.1.load129, ptr %.fca.0.1.gep426, align 4
  store i32 %.fca.0.2.load132, ptr %.fca.0.2.gep427, align 4
  store i32 %.fca.0.3.load135, ptr %.fca.0.3.gep428, align 4
  store i32 %.fca.0.4.load138, ptr %.fca.0.4.gep429, align 4
  store i32 %.fca.0.5.load141, ptr %.fca.0.5.gep430, align 4
  store i32 %.fca.0.6.load144, ptr %.fca.0.6.gep431, align 4
  store i32 %.fca.0.7.load147, ptr %.fca.0.7.gep432, align 4
  store i32 %.fca.0.8.load150, ptr %.fca.0.8.gep433, align 4
  store i32 %.fca.0.9.load153, ptr %.fca.0.9.gep434, align 4
  store i32 %.fca.1.0.load156, ptr %.fca.1.0.gep435, align 4
  store i32 %.fca.1.1.load159, ptr %.fca.1.1.gep436, align 4
  store i32 %.fca.1.2.load162, ptr %.fca.1.2.gep437, align 4
  store i32 %.fca.1.3.load165, ptr %.fca.1.3.gep438, align 4
  store i32 %.fca.1.4.load168, ptr %.fca.1.4.gep439, align 4
  store i32 %.fca.1.5.load171, ptr %.fca.1.5.gep440, align 4
  store i32 %.fca.1.6.load174, ptr %.fca.1.6.gep441, align 4
  store i32 %.fca.1.7.load177, ptr %.fca.1.7.gep442, align 4
  store i32 %.fca.1.8.load180, ptr %.fca.1.8.gep443, align 4
  store i32 %.fca.1.9.load183, ptr %.fca.1.9.gep444, align 4
  store i32 %.fca.2.0.load186, ptr %.fca.2.0.gep445, align 4
  store i32 %.fca.2.1.load189, ptr %.fca.2.1.gep446, align 4
  store i32 %.fca.2.2.load192, ptr %.fca.2.2.gep447, align 4
  store i32 %.fca.2.3.load195, ptr %.fca.2.3.gep448, align 4
  store i32 %.fca.2.4.load198, ptr %.fca.2.4.gep449, align 4
  store i32 %.fca.2.5.load201, ptr %.fca.2.5.gep450, align 4
  store i32 %.fca.2.6.load204, ptr %.fca.2.6.gep451, align 4
  store i32 %.fca.2.7.load207, ptr %.fca.2.7.gep452, align 4
  store i32 %.fca.2.8.load210, ptr %.fca.2.8.gep453, align 4
  store i32 %.fca.2.9.load213, ptr %.fca.2.9.gep454, align 4
  store i32 %.fca.3.0.load216, ptr %.fca.3.0.gep455, align 4
  store i32 %.fca.3.1.load219, ptr %.fca.3.1.gep456, align 4
  store i32 %.fca.3.2.load222, ptr %.fca.3.2.gep457, align 4
  store i32 %.fca.3.3.load225, ptr %.fca.3.3.gep458, align 4
  store i32 %.fca.3.4.load228, ptr %.fca.3.4.gep459, align 4
  store i32 %.fca.3.5.load231, ptr %.fca.3.5.gep460, align 4
  store i32 %.fca.3.6.load234, ptr %.fca.3.6.gep461, align 4
  store i32 %.fca.3.7.load237, ptr %.fca.3.7.gep462, align 4
  store i32 %.fca.3.8.load240, ptr %.fca.3.8.gep463, align 4
  store i32 %.fca.3.9.load243, ptr %.fca.3.9.gep464, align 4
  store i32 %.fca.4.0.load246, ptr %.fca.4.0.gep465, align 4
  store i32 %.fca.4.1.load249, ptr %.fca.4.1.gep466, align 4
  store i32 %.fca.4.2.load252, ptr %.fca.4.2.gep467, align 4
  store i32 %.fca.4.3.load255, ptr %.fca.4.3.gep468, align 4
  store i32 %.fca.4.4.load258, ptr %.fca.4.4.gep469, align 4
  store i32 %.fca.4.5.load261, ptr %.fca.4.5.gep470, align 4
  store i32 %.fca.4.6.load264, ptr %.fca.4.6.gep471, align 4
  store i32 %.fca.4.7.load267, ptr %.fca.4.7.gep472, align 4
  store i32 %.fca.4.8.load270, ptr %.fca.4.8.gep473, align 4
  store i32 %.fca.4.9.load273, ptr %.fca.4.9.gep474, align 4
  store i32 %.fca.5.0.load276, ptr %.fca.5.0.gep475, align 4
  store i32 %.fca.5.1.load279, ptr %.fca.5.1.gep476, align 4
  store i32 %.fca.5.2.load282, ptr %.fca.5.2.gep477, align 4
  store i32 %.fca.5.3.load285, ptr %.fca.5.3.gep478, align 4
  store i32 %.fca.5.4.load288, ptr %.fca.5.4.gep479, align 4
  store i32 %.fca.5.5.load291, ptr %.fca.5.5.gep480, align 4
  store i32 %.fca.5.6.load294, ptr %.fca.5.6.gep481, align 4
  store i32 %.fca.5.7.load297, ptr %.fca.5.7.gep482, align 4
  store i32 %.fca.5.8.load300, ptr %.fca.5.8.gep483, align 4
  store i32 %.fca.5.9.load303, ptr %.fca.5.9.gep484, align 4
  store i32 %.fca.6.0.load306, ptr %.fca.6.0.gep485, align 4
  store i32 %.fca.6.1.load309, ptr %.fca.6.1.gep486, align 4
  store i32 %.fca.6.2.load312, ptr %.fca.6.2.gep487, align 4
  store i32 %.fca.6.3.load315, ptr %.fca.6.3.gep488, align 4
  store i32 %.fca.6.4.load318, ptr %.fca.6.4.gep489, align 4
  store i32 %.fca.6.5.load321, ptr %.fca.6.5.gep490, align 4
  store i32 %.fca.6.6.load324, ptr %.fca.6.6.gep491, align 4
  store i32 %.fca.6.7.load327, ptr %.fca.6.7.gep492, align 4
  store i32 %.fca.6.8.load330, ptr %.fca.6.8.gep493, align 4
  store i32 %.fca.6.9.load333, ptr %.fca.6.9.gep494, align 4
  store i32 %.fca.7.0.load336, ptr %.fca.7.0.gep495, align 4
  store i32 %.fca.7.1.load339, ptr %.fca.7.1.gep496, align 4
  store i32 %.fca.7.2.load342, ptr %.fca.7.2.gep497, align 4
  store i32 %.fca.7.3.load345, ptr %.fca.7.3.gep498, align 4
  store i32 %.fca.7.4.load348, ptr %.fca.7.4.gep499, align 4
  store i32 %.fca.7.5.load351, ptr %.fca.7.5.gep500, align 4
  store i32 %.fca.7.6.load354, ptr %.fca.7.6.gep501, align 4
  store i32 %.fca.7.7.load357, ptr %.fca.7.7.gep502, align 4
  store i32 %.fca.7.8.load360, ptr %.fca.7.8.gep503, align 4
  store i32 %.fca.7.9.load363, ptr %.fca.7.9.gep504, align 4
  store i32 %.fca.8.0.load366, ptr %.fca.8.0.gep505, align 4
  store i32 %.fca.8.1.load369, ptr %.fca.8.1.gep506, align 4
  store i32 %.fca.8.2.load372, ptr %.fca.8.2.gep507, align 4
  store i32 %.fca.8.3.load375, ptr %.fca.8.3.gep508, align 4
  store i32 %.fca.8.4.load378, ptr %.fca.8.4.gep509, align 4
  store i32 %.fca.8.5.load381, ptr %.fca.8.5.gep510, align 4
  store i32 %.fca.8.6.load384, ptr %.fca.8.6.gep511, align 4
  store i32 %.fca.8.7.load387, ptr %.fca.8.7.gep512, align 4
  store i32 %.fca.8.8.load390, ptr %.fca.8.8.gep513, align 4
  store i32 %.fca.8.9.load393, ptr %.fca.8.9.gep514, align 4
  store i32 %.fca.9.0.load396, ptr %.fca.9.0.gep515, align 4
  store i32 %.fca.9.1.load399, ptr %.fca.9.1.gep516, align 4
  store i32 %.fca.9.2.load402, ptr %.fca.9.2.gep517, align 4
  store i32 %.fca.9.3.load405, ptr %.fca.9.3.gep518, align 4
  store i32 %.fca.9.4.load408, ptr %.fca.9.4.gep519, align 4
  store i32 %.fca.9.5.load411, ptr %.fca.9.5.gep520, align 4
  store i32 %.fca.9.6.load414, ptr %.fca.9.6.gep521, align 4
  store i32 %.fca.9.7.load417, ptr %.fca.9.7.gep522, align 4
  store i32 %.fca.9.8.load420, ptr %.fca.9.8.gep523, align 4
  store i32 %.fca.9.9.load423, ptr %.fca.9.9.gep524, align 4
  %32 = add nuw nsw i32 %generation.01129, 1
  %exitcond1140.not = icmp eq i32 %32, 5
  br i1 %exitcond1140.not, label %for.exit.L71, label %for.head.L73.preheader

for.exit.L71:                                     ; preds = %for.exit.L73
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #1

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #4

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, argmem: none, inaccessiblemem: none) }
attributes #1 = { nofree nounwind }
attributes #2 = { nofree norecurse nosync nounwind memory(none) }
attributes #3 = { noinline nounwind optnone uwtable }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #5 = { nounwind }

!0 = distinct !{!0, !1}
!1 = !{!"llvm.loop.peeled.count", i32 1}
