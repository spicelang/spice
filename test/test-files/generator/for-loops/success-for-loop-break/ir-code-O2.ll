; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 1
@1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  br label %for

for:                                              ; preds = %for, %entry
  %counter.018 = phi i32 [ 0, %entry ], [ %2, %for ]
  %0 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([13 x i8], [13 x i8]* @0, i64 0, i64 0), i32 %counter.018)
  %1 = icmp ugt i32 %counter.018, 9000
  %2 = add nuw nsw i32 %counter.018, 2
  br i1 %1, label %for1, label %for

for1:                                             ; preds = %for, %for1
  %subCounter.017 = phi i32 [ %5, %for1 ], [ 100, %for ]
  %3 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([19 x i8], [19 x i8]* @1, i64 0, i64 0), i32 %subCounter.017)
  %4 = icmp eq i32 %subCounter.017, 11
  %5 = add nsw i32 %subCounter.017, -1
  br i1 %4, label %for.end6, label %for1

for.end6:                                         ; preds = %for1
  %6 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([5 x i8], [5 x i8]* @2, i64 0, i64 0))
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
