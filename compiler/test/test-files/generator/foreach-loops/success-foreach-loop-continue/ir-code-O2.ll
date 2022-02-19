; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@l = local_unnamed_addr constant [2 x i64] [i64 1, i64 2]
@1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  %shortArray = alloca [5 x i16], align 2
  %.fca.4.insert.fca.0.gep = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i64 0, i64 0
  store i16 1, i16* %.fca.4.insert.fca.0.gep, align 2
  %.fca.4.insert.fca.1.gep = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i64 0, i64 1
  store i16 5, i16* %.fca.4.insert.fca.1.gep, align 2
  %.fca.4.insert.fca.2.gep = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i64 0, i64 2
  store i16 -2, i16* %.fca.4.insert.fca.2.gep, align 2
  %.fca.4.insert.fca.3.gep = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i64 0, i64 3
  store i16 -63, i16* %.fca.4.insert.fca.3.gep, align 2
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.inc4, %entry
  %s.0 = phi i16 [ 1, %entry ], [ %7, %foreach.inc4 ]
  %idx.0 = phi i32 [ 0, %entry ], [ %idx.inc6, %foreach.inc4 ]
  %0 = zext i16 %s.0 to i32
  %1 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([10 x i8], [10 x i8]* @0, i64 0, i64 0), i32 %0)
  %2 = and i16 %s.0, 1
  %.not = icmp eq i16 %2, 0
  br i1 %.not, label %if.end, label %if.then

if.then:                                          ; preds = %foreach.loop
  %3 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([9 x i8], [9 x i8]* @1, i64 0, i64 0), i32 1)
  br label %foreach.inc4

if.end:                                           ; preds = %foreach.loop
  %4 = icmp slt i32 %idx.0, 4
  br i1 %4, label %foreach.inc4, label %foreach.end7

foreach.inc4:                                     ; preds = %if.end, %if.then
  %idx.inc6 = add i32 %idx.0, 1
  %5 = sext i32 %idx.inc6 to i64
  %6 = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i64 0, i64 %5
  %7 = load i16, i16* %6, align 2
  br label %foreach.loop

foreach.end7:                                     ; preds = %if.end
  %8 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([5 x i8], [5 x i8]* @2, i64 0, i64 0))
  ret i32 0
}

attributes #0 = { nofree nounwind }
