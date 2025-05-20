def no_warnings():
  import warnings
  warnings.filterwarnings("ignore")
  import os
  os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3' 
  import tensorflow as tf
  tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)
