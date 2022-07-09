; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  br label %for

for:                                              ; preds = %entry, %for.inc
  %.187 = phi i64 [ 1, %entry ], [ %.0, %for.inc ]
  %.16786 = phi i64 [ 0, %entry ], [ %.066, %for.inc ]
  %.16985 = phi i64 [ 1, %entry ], [ %.068, %for.inc ]
  %.17184 = phi i64 [ 1, %entry ], [ %.070, %for.inc ]
  %.17383 = phi i64 [ 3, %entry ], [ %.072, %for.inc ]
  %.17582 = phi i64 [ 3, %entry ], [ %.074, %for.inc ]
  %.17781 = phi i32 [ 0, %entry ], [ %.076, %for.inc ]
  %.07880 = phi i32 [ 0, %entry ], [ %29, %for.inc ]
  %0 = shl i64 %.187, 2
  %1 = sub i64 %.16786, %.16985
  %2 = add i64 %1, %0
  %3 = mul i64 %.16985, %.17383
  %4 = icmp slt i64 %2, %3
  br i1 %4, label %if.then, label %if.else

if.then:                                          ; preds = %for
  %5 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @0, i64 0, i64 0), i64 %.17383)
  %6 = icmp eq i32 %.17781, 0
  br i1 %6, label %if.then1, label %if.end

if.then1:                                         ; preds = %if.then
  %putchar = tail call i32 @putchar(i32 46)
  br label %if.end

if.end:                                           ; preds = %if.then1, %if.then
  %7 = add i32 %.17781, 1
  %8 = mul i64 %.187, 10
  %9 = sub i64 %.16786, %3
  %10 = mul i64 %9, 10
  %11 = mul i64 %.187, 3
  %12 = add i64 %11, %.16786
  %13 = mul i64 %12, 10
  %14 = sdiv i64 %13, %.16985
  %.neg = mul i64 %.17383, -10
  %15 = add i64 %14, %.neg
  br label %for.inc

if.else:                                          ; preds = %for
  %16 = mul i64 %.187, %.17184
  %17 = shl i64 %.187, 1
  %18 = add i64 %17, %.16786
  %19 = mul i64 %18, %.17582
  %20 = mul i64 %.16985, %.17582
  %21 = add i64 %.17184, 1
  %22 = mul i64 %.17184, 7
  %23 = add i64 %22, 2
  %24 = mul i64 %.187, %23
  %25 = mul i64 %.16786, %.17582
  %26 = add i64 %24, %25
  %27 = sdiv i64 %26, %20
  %28 = add i64 %.17582, 2
  br label %for.inc

for.inc:                                          ; preds = %if.end, %if.else
  %.076 = phi i32 [ %7, %if.end ], [ %.17781, %if.else ]
  %.074 = phi i64 [ %.17582, %if.end ], [ %28, %if.else ]
  %.072 = phi i64 [ %15, %if.end ], [ %27, %if.else ]
  %.070 = phi i64 [ %.17184, %if.end ], [ %21, %if.else ]
  %.068 = phi i64 [ %.16985, %if.end ], [ %20, %if.else ]
  %.066 = phi i64 [ %10, %if.end ], [ %19, %if.else ]
  %.0 = phi i64 [ %8, %if.end ], [ %16, %if.else ]
  %29 = add nuw nsw i32 %.07880, 1
  %exitcond.not = icmp eq i32 %29, 20
  br i1 %exitcond.not, label %for.end, label %for

for.end:                                          ; preds = %for.inc
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
